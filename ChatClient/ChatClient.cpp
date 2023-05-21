#include "Log.h"
#include "ChatClient.h"
#include "ChatServer.h"
#include "LoginInReplyData.h"
#include "RegisterJsonData.h"
#include "RegisterReplyData.h"
#include "../PublicFunction.hpp"
#include "GetFriendListReplyData.h"
#include "InitialRequestJsonData.h"
#include "AddFriendNotifyJsonData.h"
#include "ProfileImageMsgJsonData.h"
#include "GetProfileImageJsonData.h"
#include "AddFriendRequestJsonData.h"
#include "SingleChatMessageJsonData.h"
#include "AddFriendResponseJsonData.h"
#include "StartGroupChatJsonData.h"

#include <fstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>


using namespace boost;
using namespace protocol;
namespace net
{
    constexpr int kHeartPackageTime=300;
    //初始化的时候会把socket move过来，会保存服务器的指针，会定时5分钟的心跳保活
    ChatClient::ChatClient(tcp::socket sock,ChatServer* ptr,boost::asio::io_context& ioc)
    :m_clientSocket(std::move(sock)),
    m_ptrChatServer(ptr),
    m_timer(ioc,boost::posix_time::seconds(kHeartPackageTime)),
    m_timerForStopReceiveImage(ioc,std::chrono::seconds(5))
    { 
        printf("new client\n");
        initMsgCallback();
        initImageMsgCallback();
        //m_timer.async_wait(std::bind(&ChatClient::removeSelfFromServer,this));
        m_timer.async_wait
        (
            [this](const std::error_code& ec)
            {
                //125 indicates that the operation was aborted because the timer was cancelled.
                if(0!=ec.value())
                {
                    return;
                }
                printf("no heart package\n");
                m_clientSocket.cancel();
                auto self=shared_from_this();
                //m_bReadCancel=true;
                m_ptrChatServer->removeDisconnetedClient(m_iId,self);
            }
        );
    }

    ChatClient::~ChatClient()
    {
        printf("des\n");
        m_timer.cancel();
        //运行状态标志位false，因为测试发现析构后定时器回调函数还会执行，待改进
        //取消所有异步任务
        //m_bReadCancel=true;
        m_clientSocket.cancel();
        m_clientSocket.close();
        //TODO修改这个client对应id的状态
    }

    void ChatClient::Start()
    {
        //开启之后就开始读
        DoRead();
    }

    void ChatClient::DoWrite(const std::string& message,int length)
    {
        //printf("write message:%s\n",message.c_str());
        PackageHead head;
        memcpy(head.flagBit, "&q*b", 4);
        head.length = length;
        head.cmdId = 0x0011;
        std::string strHead((char*)&head, sizeof(PackageHead));
        std::string sendMsg = strHead + message;
        int cnt=boost::asio::write(m_clientSocket,boost::asio::buffer(sendMsg.c_str(),length+sizeof(PackageHead)));
    }

    void ChatClient::DowriteImage(const std::string& ImagePath,const std::string& jsonMsgOfImage)
    {
        //先判断文件是否存在
         PackageHead head;

        long size = 0;
        FILE* fp = fopen(ImagePath.c_str(), "r");
        //如果打开失败就返回
        if (fp == NULL)
        {
            return;
        }
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fclose(fp);
        memcpy(head.flagBit, "&q*b", 4);
        if (jsonMsgOfImage.length() > 0)
        {
            head.length = size + jsonMsgOfImage.length();
        }
        else
        {
            head.length = size;
        }
        head.cmdId = 0x0012;

        std::string strHead((char*)&head, sizeof(PackageHead));
        int sends=boost::asio::write(m_clientSocket,boost::asio::buffer(strHead.c_str(), PackageHeadSize));

        if (jsonMsgOfImage.length() > 0)
        {
            sends += boost::asio::write(m_clientSocket,boost::asio::buffer(jsonMsgOfImage.c_str(),jsonMsgOfImage.length()));
        }

        //读取图片文件并发送出去
        char* imageBuf=new char[size];
        fread(imageBuf, size, 1, fp);
        fclose(fp);

        sends += boost::asio::write(m_clientSocket,boost::asio::buffer(imageBuf, size));
    }

    void ChatClient::DoRead()
    {
        //从自身得到的智能指针，避免异步读取的时候client已经析构了
        auto self=shared_from_this();
        //读取错误的信息
        boost::system::error_code ec;
        //首先清空读缓冲区
        memset(m_oneBuffer,0,FIRSTBUFFERLENGTH);
        //开启异步读任务，传递buffer和回调函数，这里用的lambda表达式
        printf("read1\n");
        m_clientSocket.async_read_some
        (
        boost::asio::buffer(m_oneBuffer,FIRSTBUFFERLENGTH),
        [this,self](std::error_code ec,size_t length)
        {
            if(!ec)
            {
                printf("read2\n");
                if(m_bImageWrite)
                {
                    m_stuRecvImageInfo.m_nImageSize -=length;
                    m_osImageWrite.write(m_oneBuffer,length);
                    if(0==m_stuRecvImageInfo.m_nImageSize)
                    {
                        m_timerForStopReceiveImage.cancel();
                        m_bImageWrite=false;
                        m_osImageWrite.close();
                        handleImageMessage(m_stuRecvImageInfo.m_strJsonMsg,m_stuRecvImageInfo.m_strImageName);
                    }
                    DoRead();
                    return;
                }
                //std::string tss(m_cBuffer,m_endPosOfBuffer);
                printf("recv:%s\n",m_oneBuffer);
                //会把从网络传递来的数据从小缓冲区存在一个大缓冲区中，在大缓冲区中进行业务处理
                memcpy(m_cBuffer+m_endPosOfBuffer,m_oneBuffer,length);
                //每次存储到大缓冲区后都要更新他的尾部标识
                m_endPosOfBuffer+=length;
                std::string str(m_cBuffer,m_endPosOfBuffer);
                printf("read len:%d\n",length);
                int pos=0;
                //要读到固定的包头才行
                while(pos<m_endPosOfBuffer && memcmp(m_cBuffer+pos,"&q*b",4)!=0)
                {
                    pos+=4;
                }
                printf("find pos %d,and end is:%d\n",pos,m_endPosOfBuffer);
                if(pos>=m_endPosOfBuffer)
                {
                    //如果没有找到包头，就把缓冲区清空
                    pos=m_endPosOfBuffer;
                }
                memcpy(m_cBuffer,m_cBuffer+pos,m_endPosOfBuffer-pos);
                m_endPosOfBuffer-=pos;
                
                while(m_endPosOfBuffer>PackageHeadSize)
                {
                    //执行到此认为长度是大于包头的，然后去获取长度
                    char lengthStr[sizeof(PackageHead)+1]{0};
                    memcpy(lengthStr,m_cBuffer,sizeof(PackageHead));
                    auto head=reinterpret_cast<PackageHead*>(lengthStr);
                    unsigned int lengthOfMessage=head->length;

                    //根据cmdid第一次分流，分别处理消息，图片，文件等
                    if(0x0011==head->cmdId)
                    {
                        //得到数据包的长度
                        //进行第一次业务处理,查看是收到的数据是否大于包头指示的长度
                        //大于的时候就处理，小于就去继续读
                        //读到指定长度为止
                        while(m_endPosOfBuffer<lengthOfMessage+PackageHeadSize)
                        {
                            memset(m_oneBuffer,0,FIRSTBUFFERLENGTH);
                            boost::system::error_code ecs;
                            int n=read(m_clientSocket,boost::asio::buffer(m_oneBuffer,FIRSTBUFFERLENGTH),ecs);
                            if(ecs)
                            {
                                m_clientSocket.cancel(); 
                                m_ptrChatServer->removeDisconnetedClient(m_iId,self);
                                return;
                            }
                            memcpy(m_cBuffer+m_endPosOfBuffer,m_oneBuffer,n);
                            m_endPosOfBuffer+=n;
                        }
                        std::string test(m_cBuffer+PackageHeadSize,lengthOfMessage);
                        //因已取出一部分信息，要把大缓冲区的内容更新一下
                        memcpy(m_cBuffer,m_cBuffer+lengthOfMessage+PackageHeadSize,BUFFERLENGTH-(lengthOfMessage+PackageHeadSize));
                        //尾部标识也更新一下
                        m_endPosOfBuffer-=(lengthOfMessage+PackageHeadSize);
                        handleClientMessage(test);
                    }
                    else if(0x0012==head->cmdId)
                    {
                        while(m_endPosOfBuffer<lengthOfMessage+PackageHeadSize)
                        {
                            memset(m_oneBuffer,0,FIRSTBUFFERLENGTH);
                            boost::system::error_code ecs;
                            int n=read(m_clientSocket,boost::asio::buffer(m_oneBuffer,FIRSTBUFFERLENGTH),ecs);
                            if(ecs)
                            {
                                m_clientSocket.cancel(); 
                                m_ptrChatServer->removeDisconnetedClient(m_iId,self);
                                return;
                            }
                            memcpy(m_cBuffer+m_endPosOfBuffer,m_oneBuffer,n);
                        }
                        //先获取到跟图片信息相关的json字符串
                        std::string tmpStr(m_cBuffer+PackageHeadSize,lengthOfMessage);
                        //因已取出一部分信息，要把大缓冲区的内容更新一下
                        memcpy(m_cBuffer,m_cBuffer+lengthOfMessage+PackageHeadSize,BUFFERLENGTH-lengthOfMessage-PackageHeadSize);
                        //尾部标识也更新一下
                        m_endPosOfBuffer-=(lengthOfMessage+PackageHeadSize);

                        m_stuRecvImageInfo.m_strJsonMsg=tmpStr;

                        protocol::image::StartGroupJsonData startGroupChatData(tmpStr);
                        m_stuRecvImageInfo.m_nImageSize=startGroupChatData.m_iImageLenth;

                        //当前时间转字符串
                        std::time_t t=std::time(nullptr);
                        std::tm* now=std::localtime(&t);
                        std::stringstream ss;
                        ss<<now->tm_year+1900<<now->tm_mon+1<<now->tm_mday<<now->tm_hour<<now->tm_min<<now->tm_sec;
                        std::string fileName=ss.str()+"."+startGroupChatData.m_strImageSuffix;
                        m_stuRecvImageInfo.m_strImageName=fileName;
                        m_osImageWrite.open(fileName.c_str(),std::ios::binary|std::ios::app);

                        char buf[m_endPosOfBuffer]{0};
                        if(m_endPosOfBuffer>0)
                        {
                            memcpy(buf,m_cBuffer,m_endPosOfBuffer);
                            m_osImageWrite.write(buf,m_endPosOfBuffer);
                            memset(m_cBuffer,0,m_endPosOfBuffer);
                            m_stuRecvImageInfo.m_nImageSize-=m_endPosOfBuffer;
                            m_endPosOfBuffer=0;
                        }
                        m_bImageWrite=true;
                        m_timerForStopReceiveImage.expires_after(std::chrono::seconds(5));
                        m_timerForStopReceiveImage.async_wait([this](const std::error_code& ec){
                            if(0==ec.value())
                            {
                                m_bImageWrite=false;
                                m_osImageWrite.close();
                            }
                            m_timerForStopReceiveImage.cancel();
                        });
                    }
                }
                DoRead();
                //业务处理完了，继续读
            }
            //读取到了错误或者断开连接
            else
            {
                //read eof or connection reset by peer
                //正常断开连接的时候会受到eof,而程序直接关闭，并且不处理socket的时候就会触发104错误
                if(boost::asio::error::eof!=ec.value())
                {
                    _LOG(Logcxx::INFO,"connection dis unnormally,error code:%d",ec.value());
                }
                
                m_clientSocket.cancel(); 
                m_ptrChatServer->removeDisconnetedClient(m_iId,self);
                return;
            }
        }
        );
    }

    bool ChatClient::praseJsonString(std::string& message,ptree& pt)
    {
        //transfer to stream
        std::stringstream ss(message);
        //read json type
        read_json(ss,pt);
        return true;
    }

    //处理客户端发送来的消息
    void ChatClient::handleClientMessage(const std::string& message)
    {
        //收到任意消息，都把定时器更新一下
        m_timer.cancel();
        //m_timer.expires_at(m_timer.expires_at()+boost::posix_time::seconds(kHeartPackageTime)-m_timer.expires_from_now());
        m_timer.expires_from_now(boost::posix_time::seconds(kHeartPackageTime));
        m_timer.async_wait
        (
            [this](const std::error_code& ec)
            {
                //printf("error code is:%d\n",ec.value());
                if(0!=ec.value())
                {
                    return;
                }
                m_clientSocket.cancel();
                auto self=shared_from_this();
                m_ptrChatServer->removeDisconnetedClient(m_iId,self);
            }
        );
        _LOG(Logcxx::INFO,"enter handle clientMessage: %s",message.c_str());
        //传递的消息类型为json格式
        //rapidjson解析json
        rapidjson::Document doc;
        doc.Parse(message.c_str());
        if(doc.HasParseError())
        {
            _LOG(Logcxx::INFO,"json parse error");
            return;
        }
        //首先获取这次得到的消息的类型
        int imessageType=doc["type"].GetInt();
        
        if(m_mapMsgHandleFunc.count(imessageType))
        {
            m_mapMsgHandleFunc[imessageType](message);
        }
        else
        {
            _LOG(Logcxx::INFO,"no such message type");
        }
        return;
    }

    void ChatClient::handleImageMessage(const std::string &imageJsonData, const std::string &imagePath)
    {
        //收到任意消息，都把定时器更新一下
        m_timer.cancel();
        //m_timer.expires_at(m_timer.expires_at()+boost::posix_time::seconds(kHeartPackageTime)-m_timer.expires_from_now());
        m_timer.expires_from_now(boost::posix_time::seconds(kHeartPackageTime));
        m_timer.async_wait
        (
            [this](const std::error_code& ec)
            {
                //printf("error code is:%d\n",ec.value());
                if(0!=ec.value())
                {
                    return;
                }
                m_clientSocket.cancel();
                auto self=shared_from_this();
                m_ptrChatServer->removeDisconnetedClient(m_iId,self);
            }
        );
        //传递的消息类型为json格式
        //rapidjson解析json
        rapidjson::Document doc;
        doc.Parse(imageJsonData.c_str());
        if(doc.HasParseError())
        {
            _LOG(Logcxx::INFO,"json parse error");
            return;
        }
        //首先获取这次得到的消息的类型
        int imessageType=doc["type"].GetInt();
        
        if(m_mapImageMsgHandleFunc.count(imessageType))
        {
            m_mapImageMsgHandleFunc[imessageType](imageJsonData,imagePath);
        }
        else
        {
            _LOG(Logcxx::INFO,"no such message type");
        }
        return;
    }

    void ChatClient::initMsgCallback()
    {
        m_mapMsgHandleFunc[static_cast<int>(MessageType::LoginRequest)]=std::bind(&ChatClient::handleLoginMessage,this,std::placeholders::_1);
        m_mapMsgHandleFunc[static_cast<int>(MessageType::RegisterRequest)]=std::bind(&ChatClient::handleRegisterMessage,this,std::placeholders::_1);
        m_mapMsgHandleFunc[static_cast<int>(MessageType::AddFriendRequest)]=std::bind(&ChatClient::handleAddFriendMessage,this,std::placeholders::_1);
        m_mapMsgHandleFunc[static_cast<int>(MessageType::AddFriendResponse)]=std::bind(&ChatClient::handleDealAddFriendMessage,this,std::placeholders::_1);
        m_mapMsgHandleFunc[static_cast<int>(MessageType::GetFriendListRequest)]=std::bind(&ChatClient::handleGetFriendListMessage,this,std::placeholders::_1);
        m_mapMsgHandleFunc[static_cast<int>(MessageType::getFriendProfileImage)]=std::bind(&ChatClient::handleGetProfileImageMessage,this,std::placeholders::_1);
        m_mapMsgHandleFunc[static_cast<int>(MessageType::HeartPackage)]=std::bind(&ChatClient::handleKeepAliveMessage,this,std::placeholders::_1);
        m_mapMsgHandleFunc[static_cast<int>(MessageType::InitialRequest)]=std::bind(&ChatClient::handleInitMessage,this,std::placeholders::_1);
        m_mapMsgHandleFunc[static_cast<int>(MessageType::SingleChat)]=std::bind(&ChatClient::handleChatMessage,this,std::placeholders::_1);
        m_mapMsgHandleFunc[static_cast<int>(MessageType::ProfileImageMsg)]=std::bind(&ChatClient::handleRevcProfileImageMessage,this,std::placeholders::_1);    
    }   

    void ChatClient::handleLoginMessage(const std::string &message)
    {
        ptree pt;
        std::stringstream ss(message);
        read_json(ss,pt);

        std::string userId=pt.get<std::string>("UserId");
        std::string password=pt.get<std::string>("UserPassword");
        bool logInState=false;
        //查询一下数据库
        if(MysqlQuery::Instance()->VertifyPassword(atoi(userId.c_str()),password))
        {
            logInState=true;
        }
        LoginInReplyData logInReplyData;
        logInReplyData.m_strUserName=MysqlQuery::Instance()->queryUserNameAcordId(userId);
        logInReplyData.m_bLoginInResult=logInState;
        std::string loginReplyMessage=logInReplyData.generateJson();
        DoWrite(loginReplyMessage,loginReplyMessage.length());
    }
    void ChatClient::handleRegisterMessage(const std::string &message)
    {
        ptree pt;
        std::stringstream ss(message);
        read_json(ss,pt);
        std::string userName=pt.get<std::string>("UserName");
        std::string password=pt.get<std::string>("UserPassword");
        bool registerState=false;
        int id=MysqlQuery::Instance()->InsertNewUser(userName,password,"");
        if(-1!=id)
        {
            registerState=true;
        }
        RegisterReplyData registerReplyData;
        registerReplyData.m_bRegisterResult=registerState;
        registerReplyData.m_iId=id;
        std::string replyMessage=registerReplyData.generateJson();
        DoWrite(replyMessage,replyMessage.length());
    }
    void ChatClient::handleChatMessage(const std::string &message)
    {
        ptree pt;
        std::stringstream ss(message);
        read_json(ss,pt);
        SingleChatMessageJsonData singleChatData(message);
        //std::string recvId=pt.get<std::string>("RecvUserId");
        //获取一下要接受消息的人的在线状态
        bool onlineState=MysqlQuery::Instance()->queryUserIsOnline(singleChatData.m_strRecvUserId);
        //如果在线，就转发
        if(onlineState)
        {
            std::string sendMessage=message;
            m_ptrChatServer->transferMessage(atoi(singleChatData.m_strRecvUserId.c_str()),sendMessage);
        }
        else
        {
            //TODO插入数据库中
            MysqlQuery::Instance()->insertCachedChatMsg(singleChatData.m_strSendUserId,singleChatData.m_strRecvUserId,singleChatData.m_strMessage,singleChatData.m_strSendName,singleChatData.m_strTime);
        }
    }
    void ChatClient::handleKeepAliveMessage(const std::string &message)
    {
    }
    void ChatClient::handleInitMessage(const std::string &message)
    {
        ptree pt;
        std::stringstream ss(message);
        read_json(ss,pt);
        //解析出消息，得到id，存储下来
        std::string userId=pt.get<std::string>("UserId");
        m_iId=atoi(userId.c_str());
        m_ptrChatServer->insertIntoIdMap(atoi(userId.c_str()),shared_from_this());
        MysqlQuery::Instance()->updateUserOnlineState(userId,true);
        //查询表获取好友列表，并发送
        GetFriendListReplyData getFriendListReplyData;
        MysqlQuery::Instance()->queryUserFrinedList(getFriendListReplyData.m_vecFriendList,userId);
        auto sendStr=getFriendListReplyData.generateJson();
        DoWrite(sendStr,getFriendListReplyData.generateJson().length());
        //TODO推送缓存的聊天消息和添加好友请求
        std::vector<MyAddFriendInfo> vecCachedAddFriend;
        MysqlQuery::Instance()->queryCachedAddFriendInfo(vecCachedAddFriend,userId);
        if(vecCachedAddFriend.size()>0)
        {
            MysqlQuery::Instance()->deleteCachedAddFriendInfo(userId);
        }
        for(auto& item:vecCachedAddFriend)
        {
            AddFriendRequestJsonData tmp;
            tmp.m_strFriendId=item.m_strFriendId;
            tmp.m_strMyId=item.m_strMyId;
            tmp.m_strName=MysqlQuery::Instance()->queryUserNameAcordId(tmp.m_strFriendId);
            tmp.m_strVerifyMsg=item.m_strVerifyMsg;
            DoWrite(tmp.generateJson(),tmp.generateJson().length());
        }
        std::vector<MyChatMessageInfo> vecCachedChatInfo;
        MysqlQuery::Instance()->queryCachedChatMsg(vecCachedChatInfo,userId);
        if(vecCachedChatInfo.size()>0)
        {
            MysqlQuery::Instance()->deleteCachedChatMsg(userId);
        }
        for(auto & item:vecCachedChatInfo)
        {
            SingleChatMessageJsonData tmp;
            tmp.m_strMessage=item.m_strChatMsg;
            tmp.m_strRecvUserId=item.m_strToId;
            tmp.m_strSendUserId=item.m_strFromId;
            tmp.m_strSendName=item.m_strSendName;
            DoWrite(tmp.generateJson(),tmp.generateJson().length());

            //把头像也发送过去
            auto imagePath=MysqlQuery::Instance()->queryImagePathAcordId(item.m_strFromId);
            std::string timeStamp=MysqlQuery::Instance()->queryImageTimeStampAcordId(item.m_strFromId);
            std::string suffix=imagePath.substr(imagePath.find_last_of('.') + 1);//获取文件后缀
            boost::uuids::uuid uid = boost::uuids::random_generator()();
            const std::string uid_str = boost::uuids::to_string(uid);
            if(imagePath!="")
            {
                std::fstream in(imagePath,std::ios::in);
                if(in.is_open())
                {
                    std::string base64Str;
                    in>>base64Str;
                    in.close();
                    int iSumIndex=base64Str.length()/kSegmentLength;
                    if(base64Str.length()%kSegmentLength!=0)
                    {
                        iSumIndex++;
                    }
                    for(int i=0;i<iSumIndex;i++)
                    {
                        ProfileImageMsgJsonData profileImageMsgJsonData;
                        profileImageMsgJsonData.m_strId=item.m_strFromId;
                        profileImageMsgJsonData.m_strUUID=uid_str;
                        profileImageMsgJsonData.m_strSuffix=suffix;
                        profileImageMsgJsonData.m_strTimeStamp=timeStamp;
                        profileImageMsgJsonData.m_iCurIndex=i+1;
                        profileImageMsgJsonData.m_iSumIndex=iSumIndex;
                        profileImageMsgJsonData.m_strBase64Msg=base64Str.substr(i*kSegmentLength,kSegmentLength);
                        profileImageMsgJsonData.m_eImageType=ProfileImageType::AddFriendProfileImage;
                        auto sendStr=profileImageMsgJsonData.generateJson();
                        DoWrite(sendStr,sendStr.length());
                    }
                }
                else
                {
                    _LOG(Logcxx::Level::ERROR,u8"打开文件失败");
                }
            }
            else
            {
                _LOG(Logcxx::Level::INFO,u8"数据库中没有该用户的头像");
            }
        }
    }
    void ChatClient::handleDealAddFriendMessage(const std::string &message)
    {
        ptree pt;
        std::stringstream ss(message);
        read_json(ss,pt);
        AddFriendResponseJsonData addFriendResponseData(message);
        //如果同意，双方的好友库里增加好友信息
        if(addFriendResponseData.m_bResult)
        {
            //通知双方你们已经是好友了
            AddFriendNotify addFriendNotifyData;
            addFriendNotifyData.m_strId1=addFriendResponseData.m_strFriendId;
            addFriendNotifyData.m_strId2=addFriendResponseData.m_strMyId;
            addFriendNotifyData.m_strName1=MysqlQuery::Instance()->queryUserNameAcordId(addFriendResponseData.m_strFriendId);
            addFriendNotifyData.m_strName2=MysqlQuery::Instance()->queryUserNameAcordId(addFriendResponseData.m_strMyId);
            addFriendNotifyData.m_strImageStamp1=MysqlQuery::Instance()->queryImageTimeStampAcordId(addFriendResponseData.m_strFriendId);
            addFriendNotifyData.m_strImageStamp2=MysqlQuery::Instance()->queryImageTimeStampAcordId(addFriendResponseData.m_strMyId);
            auto sendStr=addFriendNotifyData.generateJson();
            //通知到好友
            //查看好友是否在线，在线就通知
            if(MysqlQuery::Instance()->queryUserIsOnline(addFriendResponseData.m_strFriendId))
            {
                m_ptrChatServer->transferMessage(atoi(addFriendResponseData.m_strFriendId.c_str()),sendStr);
            }
            //通知到自己
            DoWrite(sendStr,sendStr.length());
            MysqlQuery::Instance()->AddFriend(addFriendResponseData.m_strFriendId,addFriendResponseData.m_strMyId,addFriendNotifyData.m_strName1,addFriendNotifyData.m_strName2);
        }
    }
    void ChatClient::handleAddFriendMessage(const std::string &message)
    {
        ptree pt;
        std::stringstream ss(message);
        read_json(ss,pt);
        AddFriendRequestJsonData addFriendRequestData(message);
        //查看用户是否存在
        if(MysqlQuery::Instance()->queryUserIsExist(addFriendRequestData.m_strFriendId))
        {
            if(MysqlQuery::Instance()->queryUserIsOnline(addFriendRequestData.m_strFriendId))
            {
                //如果在线，就直接转发消息就行了
                auto msg=message;
                m_ptrChatServer->transferMessage(atoi(addFriendRequestData.m_strFriendId.c_str()),msg);

                //把头像也发送过去
                auto imagePath=MysqlQuery::Instance()->queryImagePathAcordId(addFriendRequestData.m_strMyId);
                std::string timeStamp=MysqlQuery::Instance()->queryImageTimeStampAcordId(addFriendRequestData.m_strMyId);
                std::string suffix=imagePath.substr(imagePath.find_last_of('.') + 1);//获取文件后缀
                boost::uuids::uuid uid = boost::uuids::random_generator()();
                const std::string uid_str = boost::uuids::to_string(uid);
                if(imagePath!="")
                {
                    std::fstream in(imagePath,std::ios::in);
                    if(in.is_open())
                    {
                        std::string base64Str;
                        in>>base64Str;
                        in.close();
                        int iSumIndex=base64Str.length()/kSegmentLength;
                        if(base64Str.length()%kSegmentLength!=0)
                        {
                            iSumIndex++;
                        }
                        for(int i=0;i<iSumIndex;i++)
                        {
                            ProfileImageMsgJsonData profileImageMsgJsonData;
                            profileImageMsgJsonData.m_strId=addFriendRequestData.m_strMyId;
                            profileImageMsgJsonData.m_strUUID=uid_str;
                            profileImageMsgJsonData.m_strSuffix=suffix;
                            profileImageMsgJsonData.m_strTimeStamp=timeStamp;
                            profileImageMsgJsonData.m_iCurIndex=i+1;
                            profileImageMsgJsonData.m_iSumIndex=iSumIndex;
                            profileImageMsgJsonData.m_strBase64Msg=base64Str.substr(i*kSegmentLength,kSegmentLength);
                            profileImageMsgJsonData.m_eImageType=ProfileImageType::AddFriendProfileImage;
                            auto sendStr=profileImageMsgJsonData.generateJson();
                            m_ptrChatServer->transferMessage(atoi(addFriendRequestData.m_strFriendId.c_str()),sendStr);
                        }
                    }
                    else
                    {
                        _LOG(Logcxx::Level::ERROR,u8"打开文件失败");
                    }
                }
                else
                {
                    _LOG(Logcxx::Level::ERROR,u8"数据库中没有该用户的头像");
                }
            }
            else
            {
                //存储在数据库中，上线后推送
                MysqlQuery::Instance()->insertAddFriendCache(addFriendRequestData.m_strMyId,addFriendRequestData.m_strFriendId,addFriendRequestData.m_strVerifyMsg);
            }
        }
    }
    void ChatClient::handleGetFriendListMessage(const std::string &message)
    {
        ptree pt;
        std::stringstream ss(message);
        read_json(ss,pt);
        std::string userId=pt.get<std::string>("UserId");
            //TODO查询表获取好友列表
            GetFriendListReplyData getFriendListReplyData;
            MysqlQuery::Instance()->queryUserFrinedList(getFriendListReplyData.m_vecFriendList,userId);
            auto sendStr=getFriendListReplyData.generateJson();
            DoWrite(sendStr,getFriendListReplyData.generateJson().length());
    }
    void ChatClient::handleRevcProfileImageMessage(const std::string &message)
    {
        ptree pt;
        std::stringstream ss(message);
        read_json(ss,pt);
        ProfileImageMsgJsonData profileImageMsgData(message);
        int iNeedSegment=profileImageMsgData.m_iSumIndex;
        if(m_mapImageUUIDAndSegment.count(profileImageMsgData.m_strUUID)&&profileImageMsgData.m_iCurIndex-1 != m_mapImageUUIDAndSegment[profileImageMsgData.m_strUUID])
        {
            //TODO 回复一个uuid发送失败的消息
            m_mapImageUUIDAndBase64.erase(profileImageMsgData.m_strUUID);
            m_mapImageUUIDAndSegment.erase(profileImageMsgData.m_strUUID);
        }
        m_mapImageUUIDAndBase64[profileImageMsgData.m_strUUID]+=profileImageMsgData.m_strBase64Msg;
        if(profileImageMsgData.m_iCurIndex==profileImageMsgData.m_iSumIndex)
        {
            //如果收到的片数到达了最后一个了
            //将图片保存到本地，并将图片的路径保存到数据库中
            std::string curPath=getCurrentDir();
            //TODO从数据库获取上次的路径，删除上次的图片
            std::string oldPath=MysqlQuery::Instance()->queryImagePathAcordId(profileImageMsgData.m_strId);
            if(oldPath!="")
            {
                remove(oldPath.c_str());
            }
            curPath+="/data/profileImage/"+profileImageMsgData.m_strImageName+"."+profileImageMsgData.m_strSuffix;
            std::fstream out(curPath,std::ios::out);
            if(out.is_open())
            {
                out<<m_mapImageUUIDAndBase64[profileImageMsgData.m_strUUID];
                out.close();
            }
            else{
                _LOG(Logcxx::Level::ERROR,"保存头像时，打开文件失败");
            }
            MysqlQuery::Instance()->updateImagePathAcordId(profileImageMsgData.m_strId,curPath,profileImageMsgData.m_strTimeStamp);
            m_mapImageUUIDAndBase64.erase(profileImageMsgData.m_strUUID);
            m_mapImageUUIDAndSegment.erase(profileImageMsgData.m_strUUID);
            //TODO 回复一个发送成功的消息
        }
        m_mapImageUUIDAndSegment[profileImageMsgData.m_strUUID]=profileImageMsgData.m_iCurIndex;
    }
    void ChatClient::handleGetProfileImageMessage(const std::string &message)
    {
        ptree pt;
        std::stringstream ss(message);
        read_json(ss,pt);
        GetProfileImageJsonData getFriendProfileImageJsonData(message);
        std::string imagePath=MysqlQuery::Instance()->queryImagePathAcordId(getFriendProfileImageJsonData.m_strId);
        std::string timeStamp=MysqlQuery::Instance()->queryImageTimeStampAcordId(getFriendProfileImageJsonData.m_strId);
        std::string suffix=imagePath.substr(imagePath.find_last_of('.') + 1);//获取文件后缀
        //printf("id:%s,timestamp:%s,suffix:%s",getFriendProfileImageJsonData.m_strId.c_str(),timeStamp.c_str(),suffix.c_str());
        boost::uuids::uuid uid = boost::uuids::random_generator()();
        const std::string uid_str = boost::uuids::to_string(uid);
        if(imagePath!="")
        {
            std::fstream in(imagePath,std::ios::in);
            if(in.is_open())
            {
                std::string base64Str;
                in>>base64Str;
                in.close();
                int iSumIndex=base64Str.length()/kSegmentLength;
                if(base64Str.length()%kSegmentLength!=0)
                {
                    iSumIndex++;
                }
                for(int i=0;i<iSumIndex;i++)
                {
                    ProfileImageMsgJsonData profileImageMsgJsonData;
                    profileImageMsgJsonData.m_strId=getFriendProfileImageJsonData.m_strId;
                    profileImageMsgJsonData.m_strUUID=uid_str;
                    profileImageMsgJsonData.m_strSuffix=suffix;
                    profileImageMsgJsonData.m_strTimeStamp=timeStamp;
                    profileImageMsgJsonData.m_iCurIndex=i+1;
                    profileImageMsgJsonData.m_iSumIndex=iSumIndex;
                    profileImageMsgJsonData.m_strBase64Msg=base64Str.substr(i*kSegmentLength,kSegmentLength);
                    auto sendStr=profileImageMsgJsonData.generateJson();
                    DoWrite(sendStr,profileImageMsgJsonData.generateJson().length());
                }
            }
            else{
                _LOG(Logcxx::Level::ERROR,"open file failed");
            }
        }
        else{
            _LOG(Logcxx::Level::INFO,"image not contain in database");
        }
    }
    void ChatClient::initImageMsgCallback()
    {
        m_mapImageMsgHandleFunc[static_cast<int>(MessageType::InfoWithImage)]=(std::bind(&ChatClient::handleStartGroupChatMessage,this,std::placeholders::_1,std::placeholders::_2));
    }
    void ChatClient::handleStartGroupChatMessage(const std::string &message, const std::string &imagePath)
    {
    }
}
