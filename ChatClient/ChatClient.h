#pragma once

#include<boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include<memory>
#include<mutex>
#include<type_traits>
#include"MysqlQuery.h"

using namespace boost::property_tree;

struct LengthInfo
{
    int length;
    LengthInfo(int l):length(l){}
};

enum class MESSAGE_TYPE
{
    INIT_SECTION,
    CAHT_SINGLE_USER,
    CHAT_GROUP,
    ADD_FRIEND,
    KEEP_ALIVE
};

constexpr int BUFFERLENGTH=10*1024;

using message_type_base_type=std::underlying_type<MESSAGE_TYPE>::type;

using namespace boost::asio;
using namespace boost::asio::ip;

class ChatClient:public std::enable_shared_from_this<ChatClient>
{
public:
    ChatClient(tcp::socket sock);
    void Start();
    void DoRead();
    void DoWrite();
private:
    tcp::socket m_clientSocket;     //socket used to communication
    char m_cBuffer[BUFFERLENGTH];   //buffer to store message

    int m_endPosOfBuffer{0};
    char m_oneBuffer[1024]{0};

    //used to communication with database
    MysqlQuery* m_pMysqlQuery{nullptr};

    bool praseJsonString(std::string& message,ptree& pt);
};