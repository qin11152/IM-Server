#include "ChatClient.h"
#include "ChatServer.h"
#include "RegisterJsonData.h"
#include "RegisterReplyData.h"


//初始化的时候会把socketmove过来，会保存服务器的指针，会定时5分钟的心跳保活
ChatClient::ChatClient(tcp::socket sock,ChatServer* ptr,boost::asio::io_context& ioc)
:m_clientSocket(std::move(sock)),
m_ptrChatServer(ptr),
m_timer(ioc,boost::posix_time::seconds(300))
{ 
    m_timer.async_wait(std::bind(&ChatClient::removeSelfFromServer,this));
    m_bRunning=true;
}

ChatClient::~ChatClient()
{
    m_timer.cancel();
    //运行状态标志位false，因为测试发现析构后定时器回调函数还会执行，待改进
    m_bRunning=false;
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
    int cnt=boost::asio::write(m_clientSocket,boost::asio::buffer(message.c_str(),length));
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
            //length indicate the lenth of recevied message
                if(!ec)
                {
                    //会把从网络传递来的数据从小缓冲区存在一个大缓冲区中，在大缓冲区中进行业务处理
                    memcpy(m_cBuffer+m_endPosOfBuffer,m_oneBuffer,length);
                    printf("message length: %d\n",length);
                    //每次存储到大缓冲区后都要更新他的尾部标识
                    m_endPosOfBuffer+=length;
                    //传递来的数据都有一个包含数据长度的包头，包头是lengthinfo的结构体，当缓冲区长度大于结构体长度时
                    //进行第一次业务处理
                    if(m_endPosOfBuffer>=sizeof(LengthInfo))
                    {
                        //得到这个结构体
                        char headBuffer[sizeof(LengthInfo)]{0};
                        memcpy(headBuffer,m_cBuffer,sizeof(LengthInfo));
                        //转换一下类型并得到这次信息的长度
                        int lengthOfMessage=reinterpret_cast<LengthInfo*>(&headBuffer)->length;
                        printf("message length: %d\n",lengthOfMessage);
                        //当长度大于包头中指示的信息长度时，进行第二次业务处理
                        if(m_endPosOfBuffer>=sizeof(LengthInfo)+lengthOfMessage)
                        {
                            //从buffer中取出这个长度的信息
                            char message[lengthOfMessage+1]{0};
                            memcpy(message,m_cBuffer+sizeof(LengthInfo),lengthOfMessage);
                            std::string test(message);
                            printf("%s\n",test.c_str());
                            //因已取出一部分信息，要把大缓冲区的内容更新一下
                            memcpy(m_cBuffer,m_cBuffer+sizeof(LengthInfo)+lengthOfMessage,BUFFERLENGTH-(sizeof(LengthInfo)+lengthOfMessage));
                            //尾部标识也更新一下
                            m_endPosOfBuffer-=(sizeof(LengthInfo)+lengthOfMessage);
                            handleClientMessage(test);
                            DoRead();
                            return;
                        }
                        //如果长度小于包头指示的长度就继续读，知道得到足够的长度
                        else
                        {
                            DoRead();
                            return;
                        }
                    }
                    //如果比包头结构体都短，也是继续读
                    else
                    {
                        DoRead();
                        return;
                    }
                }
                //读取到了错误或者断开连接
                else
                {
                    //TODO read error and do something
                    printf("error code%d\n",ec.value());
                    m_bRunning=false;
                    if(104==ec.value()||2==ec.value())
                    {
                        //主动断开连接时，关闭socket，socket关闭后会立刻执行定时器的回调函数
                        m_clientSocket.close();
                        auto self=shared_from_this();
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
    if(m_bRunning==false)
    {
        return;
    }
    if(m_bFlag==true)
    {
        m_bFlag=false;
        m_timer.expires_at(m_timer.expires_at()+boost::posix_time::seconds(5)-m_timer.expires_from_now());
        m_timer.async_wait(std::bind(&ChatClient::removeSelfFromServer,this));
    }
    else
    {
        m_clientSocket.close();
        auto self=shared_from_this();
        m_ptrChatServer->removeDisconnetedClient(m_iId,self);
    }
}

void ChatClient::handleClientMessage(const std::string& message)
{
    //传递的消息类型为json格式
    //同ptree来解析
    ptree pt;
    std::stringstream ss(message);
    read_json(ss,pt);

    //首先获取这次得到的消息的类型
    int imessageType=pt.get<int>("type");
    printf("type is::%d\n",imessageType);
    //根据消息的类型做相应的处理
    switch (imessageType)
    {
    case static_cast<message_type_base_type>(MessageType::RegisterRequest):
        {
            std::string userName=pt.get<std::string>("UserName");
            std::string password=pt.get<std::string>("UserPassword");
            bool registerState=false;
            if(MysqlQuery::Instance()->InsertNewUser(userName,password,""))
            {
                registerState=true;
            }
            RegisterReplyData registerReplyData;
            registerReplyData.m_bRegisterResult=registerState;
            std::string replyMessage=registerReplyData.generateJson();
            DoWrite(replyMessage,replyMessage.length());
        }
        break;
    case static_cast<message_type_base_type>(MESSAGE_TYPE::ADD_FRIEND):
        break;
    default:
        break;
    }
    printf("handle finish\n");
    return;
}

