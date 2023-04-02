/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-03-21 20:56:40
 * @FilePath: /IM-Server/ChatServer/ChatServer.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "ChatClient.h"

#include <vector>
#include <unordered_map>

namespace net
{
    class ChatServer
    {
    public:
        ChatServer(io_context& io_ctx,short port);
        void DoAccept();
        void removeDisconnetedClient(int id,std::shared_ptr<ChatClient> ptr);
        bool ChangeUserStateInDB(int id);
        bool transferMessage(int id,std::string& message);
        //发送初始化消息后把client保存在id对应的map中
        void insertIntoIdMap(int id,std::shared_ptr<ChatClient> m_clientPtr);
    private:
        tcp::acceptor m_acceptor;
        //std::vector<std::shared_ptr<ChatClient>> m_socketVec;   //存储的是连接上来的socket，不一定有id，可能是注册的时候连接上来的
        std::unordered_map<int,std::shared_ptr<ChatClient>> m_idMap;//每个socket对应的id，也就是对应的用户id
        boost::asio::io_context& m_ioc;
    };
}
