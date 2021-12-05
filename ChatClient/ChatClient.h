#pragma once

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include<memory>
#include<mutex>
#include<type_traits>
#include"MysqlQuery.h"

using namespace boost::property_tree;

class ChatServer;

struct LengthInfo
{
    int length;
    LengthInfo(int l):length(l){}
};
//传递的消息类型都在这些枚举类型中
enum class MESSAGE_TYPE
{
    INIT_SECTION,
    CAHT_SINGLE_USER,
    CHAT_GROUP,
    ADD_FRIEND,
    KEEP_ALIVE
};

constexpr int BUFFERLENGTH=10*1024;
//消息类型的基本类型
using message_type_base_type=std::underlying_type<MESSAGE_TYPE>::type;

using namespace boost::asio;
using namespace boost::asio::ip;

class ChatClient:public std::enable_shared_from_this<ChatClient>
{
public:
    ChatClient(tcp::socket sock,ChatServer* ptr,boost::asio::io_context& ioc);
    ~ChatClient();
    void Start();
    void DoRead();
    void DoWrite(const std::string& message,int length);
private:
    tcp::socket m_clientSocket;     //新客户端链接上来的socket
    char m_cBuffer[BUFFERLENGTH];   //用来存储消息
    int m_iId{-1};                  //这是后来用户传过来的自己在服务器中的id
    bool m_bFlag{false};            //该值表示保活到时的时候有没有消息来过，有就是true
    bool m_bRunning{false};
    int m_endPosOfBuffer{0};
    char m_oneBuffer[1024]{0};

    //服务器指针，用来对服务器中的map操作
    ChatServer* m_ptrChatServer{nullptr};
    //计时器用来心跳保活
    boost::asio::deadline_timer m_timer;

    //解析json字符串用函数
    bool praseJsonString(std::string& message,ptree& pt);
    //保活定时器到时间了就移除服务器列表中的自己
    void removeSelfFromServer();
    //处理客户端消息函数
    void handleClientMessage(const std::string& message);
};