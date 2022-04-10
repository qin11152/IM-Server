#include "ChatClient.h"
#include "ChatServer.h"
#include "RegisterJsonData.h"
#include "RegisterReplyData.h"
#include "LoginInReplyData.h"

constexpr int kHeartPackageTime=300;
//初始化的时候会把socket move过来，会保存服务器的指针，会定时5分钟的心跳保活
ChatClient::ChatClient(tcp::socket sock,ChatServer* ptr,boost::asio::io_context& ioc)
:m_clientSocket(std::move(sock)),
m_ptrChatServer(ptr),
m_timer(ioc,boost::posix_time::seconds(kHeartPackageTime))
{ 
    m_timer.async_wait(std::bind(&ChatClient::removeSelfFromServer,this));
}

ChatClient::~ChatClient()
{
    m_cancel=true;
    m_timer.cancel();
    //运行状态标志位false，因为测试发现析构后定时器回调函数还会执行，待改进
    //取消所有异步任务
    m_bReadCancel=true;
    m_clientSocket.cancel();
    m_clientSocket.close();
    printf("ChatClient des\n");
}

void ChatClient::Start()
{
    //开启之后就开始读
    DoRead();
}

void ChatClient::DoWrite(const std::string& message,int length)
{
    printf("message is: %s\n",message.c_str());
    std::string msgLength=std::to_string(length);
    if(msgLength.length()<PackageHeadSize)
    {
        std::string tmpStr(PackageHeadSize-msgLength.length(),'0');   
        msgLength=tmpStr+msgLength;
    }
    msgLength=msgLength+message;
    int cnt=boost::asio::write(m_clientSocket,boost::asio::buffer(msgLength.c_str(),length+PackageHeadSize));
    printf("send %d byte\n",cnt);
    //auto self=shared_from_this();
    /*LengthInfo l(length);
    char lengthbuffer[sizeof(LengthInfo)+length]{ 0 };
	memcpy(lengthbuffer, &l, sizeof(LengthInfo));
    memcpy(lengthbuffer+4,&message,length);*/
    /*boost::asio::async_write(m_clientSocket,boost::asio::buffer(message.c_str(),length),
            [this](boost::system::error_code ec){
                if(!ec)
                {
                    printf("send message\n");
                }
            });*/
    /*m_clientSocket.async_write_some(boost::asio::buffer(lengthbuffer,4+length),[this](boost::system::error_code ec){
        if(!ec)
        {
            printf("send message\n");
        }
    });*/
}

void ChatClient::DoRead()
{
    //从自身得到的智能指针，避免异步读取的时候client已经析构了
    printf("enter doread function\n");
    auto self=shared_from_this();
    //读取错误的信息
    boost::system::error_code ec;
    //首先清空读缓冲区
    memset(m_oneBuffer,0,1024);
    //开启异步读任务，传递buffer和回调函数，这里用的lambda表达式
    m_clientSocket.async_read_some
    (
        boost::asio::buffer(m_oneBuffer,1024),
        [this,self](std::error_code ec,size_t length)
        {
            printf("enter read callback\n");
            if(m_bReadCancel==true)
            {
                return;
            }
            //length indicate the lenth of recevied message
                if(!ec)
                {
                    //会把从网络传递来的数据从小缓冲区存在一个大缓冲区中，在大缓冲区中进行业务处理
                    memcpy(m_cBuffer+m_endPosOfBuffer,m_oneBuffer,length);
                    printf("message length: %d,message is:%s\n",length,m_cBuffer);
                    //每次存储到大缓冲区后都要更新他的尾部标识
                    m_endPosOfBuffer+=length;

                    //一直等待，直到读到了足够的字节数
                    if(m_endPosOfBuffer<PackageHeadSize)
                    {
                        //清空一下从底层接受消息的缓冲区
                        memset(m_oneBuffer,0,1024);
                        //然后继续读取
                        DoRead();
                    }
                    //当数据大于包头长度的时候就要一直去处理
                    while(m_endPosOfBuffer>PackageHeadSize)
                    {
                    //执行到此认为长度是大于包头的，然后去获取长度
                    char lengthBuf[PackageHeadSize+1]{0};
                    memcpy(lengthBuf,m_cBuffer,PackageHeadSize);
                    //得到数据包的长度
                    int lengthOfMessage=atoi(lengthBuf);
                    printf("package length is%d\n",lengthOfMessage);
                    //进行第一次业务处理,查看是收到的数据是否大于包头指示的长度
                    //大于的时候就处理，小于就去继续读
                    if(m_endPosOfBuffer>=lengthOfMessage+PackageHeadSize)
                    {
                        //从buffer中取出这个长度的信息
                        char message[lengthOfMessage+1]{0};
                        memcpy(message,m_cBuffer+PackageHeadSize,lengthOfMessage);
                        std::string test(message);
                        printf("%s\n",test.c_str());
                        //因已取出一部分信息，要把大缓冲区的内容更新一下
                        memcpy(m_cBuffer,m_cBuffer+lengthOfMessage+PackageHeadSize,BUFFERLENGTH-lengthOfMessage-PackageHeadSize);
                        //尾部标识也更新一下
                        m_endPosOfBuffer-=(lengthOfMessage+PackageHeadSize);
                        handleClientMessage(test);
                    }
                    else{
                        //小于的时候，包头还是留在缓冲区，然后继续去读
                        //下次再进来还是先处理前面的包头
                        break;
                    }
                    }
                    //业务处理完了，继续读
                    DoRead();
                }
                //读取到了错误或者断开连接
                else
                {
                    //TODO read error and do something
                    printf("error code%d\n",ec.value());
                    if(104==ec.value()||2==ec.value())
                    {
                        //主动断开连接时，关闭socket，socket关闭后会立刻执行定时器的回调函数
                        m_clientSocket.cancel();
                        auto self=shared_from_this();
                        m_bReadCancel=true;
                        m_ptrChatServer->removeDisconnetedClient(m_iId,self);
                    }
                    return;
                }
        }
    );
    printf("leave do read function\n");
}

bool ChatClient::praseJsonString(std::string& message,ptree& pt)
{
    //transfer to stream
    std::stringstream ss(message);
    //read json type
    read_json(ss,pt);
}

void ChatClient::removeSelfFromServer()
{
    printf("timeout\n");
    if(m_cancel)
    {
        m_cancel=false;
        return;
    }
    m_clientSocket.close();
    auto self=shared_from_this();
    m_ptrChatServer->removeDisconnetedClient(m_iId,self);
}

void ChatClient::handleClientMessage(const std::string& message)
{
    //传递的消息类型为json格式
    //同ptree来解析
    ptree pt;
    std::stringstream ss(message);
    read_json(ss,pt);

    if(false)
    {
        std::string s="qqq";
        m_ptrChatServer->transferMessage(1,s);
    }

    //首先获取这次得到的消息的类型
    int imessageType=pt.get<int>("type");
    printf("type is::%d\n",imessageType);
    //根据消息的类型做相应的处理
    switch (imessageType)
    {
        //收到注册类型的消息
    case static_cast<message_type_base_type>(MessageType::RegisterRequest):
        {
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
        break;
        //收到添加朋友类型的消息
    case static_cast<message_type_base_type>(MESSAGE_TYPE::AddFriendRequest):
    {}
        break;
        //登录请求
    case static_cast<message_type_base_type>(MESSAGE_TYPE::LoginRequest):
    {
        std::string userId=pt.get<std::string>("UserId");
        std::string password=pt.get<std::string>("UserPassword");
        bool logInState=false;
        //查询一下数据库
        if(MysqlQuery::Instance()->VertifyPassword(atoi(userId.c_str()),password))
        {
            logInState=true;
        }
        LoginInReplyData logInReplyData;
        logInReplyData.m_bLoginInResult=logInState;
        std::string loginReplyMessage=logInReplyData.generateJson();
        DoWrite(loginReplyMessage,loginReplyMessage.length());
    }
        break;
    case static_cast<message_type_base_type>(MESSAGE_TYPE::HeartPackage):
        {
            //TODO回复心跳包,现在暂时不回复，收到了只是把标志位改一下
            m_timer.cancel();
            m_cancel=true;
            m_timer.expires_at(m_timer.expires_at()+boost::posix_time::seconds(kHeartPackageTime)-m_timer.expires_from_now());
            m_timer.async_wait(std::bind(&ChatClient::removeSelfFromServer,this));
        }
        break;
    case static_cast<message_type_base_type>(MESSAGE_TYPE::InitialRequest):
        {
            //解析出消息，得到id，存储下来
            //TODO 解析出id
            m_ptrChatServer->insertIntoIdMap(m_iId,shared_from_this());
        }
    default:
        break;
    }
    printf("handle finish\n");
    return;
}

