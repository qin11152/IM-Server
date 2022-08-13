#pragma once

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <type_traits>
#include "MysqlQuery.h"

using namespace boost::property_tree;

class ChatServer;

constexpr int BUFFERLENGTH=10*1024;
constexpr int FIRSTBUFFERLENGTH=1024;

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
    bool m_cancel{false};           //表示定时器任务是否被取消了
    bool m_bReadCancel{false};
    size_t m_endPosOfBuffer{0};
    char m_oneBuffer[FIRSTBUFFERLENGTH]{0};

    std::unordered_map<std::string,std::string> m_mapImageUUIDAndBase64;
    std::unordered_map<std::string,int> m_mapImageUUIDAndSegment;

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