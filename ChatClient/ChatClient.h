/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-05 17:03:53
 * @FilePath: /src/IM-Server/ChatClient/ChatClient.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include "MysqlQuery.h"

#include <mutex>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

namespace net
{
    class ChatServer;

    using MsgHandleFunc=std::function<void(const std::string& message)>;
    using ImageMsgHandleFunc=std::function<void(const std::string& message,const std::string& imagePath)>;

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
        
        /// @brief 发送图片消息
        /// @param ImagePath：图片路径
        /// @param jsonMsgOfImage：跟随图片消息的json字符串
        void DowriteImage(const std::string& ImagePath,const std::string& jsonMsgOfImage);

    //私有函数
    private:
        bool praseJsonString(std::string& message,ptree& pt);       //解析json字符串用函数
        void handleClientMessage(const std::string& message);       //处理客户端消息函数

        void handleImageMessage(const std::string& imageJsonData,const std::string& imagePath);

        //消息处理函数
        void initMsgCallback();
        void handleLoginMessage(const std::string& message);        //处理登录消息
        void handleRegisterMessage(const std::string& message);     //处理注册消息
        void handleChatMessage(const std::string& message);         //处理聊天消息
        void handleKeepAliveMessage(const std::string& message);    //处理保活消息
        void handleInitMessage(const std::string& message);         //处理初始化消息
        void handleDealAddFriendMessage(const std::string& message);//处理处理添加好友消息
        void handleAddFriendMessage(const std::string& message);    //处理添加好友消息
        void handleGetFriendListMessage(const std::string& message);//处理获取好友列表消息
        void handleRevcProfileImageMessage(const std::string& message);  //处理接收头像消息
        void handleGetProfileImageMessage(const std::string& message);  //处理获取头像消息

        //图片处理函数
        void initImageMsgCallback();
        void handleStartGroupChatMessage(const std::string& message,const std::string& imagePath);   //处理开始群聊消息
    
    private:
        tcp::socket m_clientSocket;     //新客户端链接上来的socket
        char m_cBuffer[BUFFERLENGTH];   //用来存储消息
        int m_iId{-1};                  //这是后来用户传过来的自己在服务器中的id
        bool m_cancel{false};           //表示定时器任务是否被取消了
        size_t m_endPosOfBuffer{0};
        char m_oneBuffer[FIRSTBUFFERLENGTH]{0};
        ChatServer* m_ptrChatServer{nullptr};       //服务器指针，用来对服务器中的map操作
        boost::asio::deadline_timer m_timer;    //计时器用来心跳保活

        MyRecvImageInfo m_stuRecvImageInfo;     //用来接收图片信息
        bool m_bImageWrite{false};      //标志正在写图片
        std::ofstream m_osImageWrite;   //图片写操作
        boost::asio::steady_timer m_timerForStopReceiveImage;     //用于停止接收图片的定时器

        std::unordered_map<std::string,std::string> m_mapImageUUIDAndBase64;
        std::unordered_map<std::string,int> m_mapImageUUIDAndSegment;
        std::unordered_map<int,MsgHandleFunc> m_mapMsgHandleFunc;      //消息处理函数map
        std::unordered_map<int,ImageMsgHandleFunc> m_mapImageMsgHandleFunc;      //图片消息处理函数map
    };
}