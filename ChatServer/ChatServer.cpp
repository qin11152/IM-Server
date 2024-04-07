/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-10-11 22:02:07
 * @FilePath: /IM-Server/ChatServer/ChatServer.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE/
 */
#include "Log.h"
#include "ChatServer.h"

namespace net
{
    ChatServer::ChatServer(io_context& io_ctx,short port)
    :m_ioc(io_ctx),
    m_acceptor(io_ctx,tcp::endpoint(tcp::v4(),port))
    {
        DoAccept();
    }

    void ChatServer::DoAccept()
    {
        printf("DoAccept\n");
        m_acceptor.async_accept(
            [this](std::error_code ec,tcp::socket socket)
            {
                if(!ec)
                {
                    auto new_Acceptor=std::make_shared<ChatClient>(std::move(socket),this,m_ioc);
                    //不用保存，doread时会shared_from_this，安全
                    //m_socketVec.push_back(new_Acceptor);
                    new_Acceptor->Start();
                }
                DoAccept();
            }
        );
    }

    void ChatServer::removeDisconnetedClient(int id,std::shared_ptr<ChatClient> ptr)
    {
        database::UserInfoTable userInfoTable;
        userInfoTable.updateUserOnlineState(std::to_string(id),false);
        if(m_idMap.find(id)!=m_idMap.end())
        {
            m_idMap.erase(id);
        }
        // for(auto iter=m_socketVec.begin();iter!=m_socketVec.end();++iter)
        // {
        //     if(*iter==ptr)
        //     {
        //         m_socketVec.erase(iter);
        //         return;
        //     }
        // }
    }

    void ChatServer::insertIntoIdMap(int id,std::shared_ptr<ChatClient> m_clientPtr)
    {
        //这里不是为了保证连接的存在，是为了转发的时候根据id能找到连接
        m_idMap[id]=m_clientPtr;
    }

    bool ChatServer::transferMessage(int id,std::string& message)
    {
        if(m_idMap.find(id)!=m_idMap.end())
        {
            m_idMap[id]->DoWrite(message,message.length());
        }
        else 
        {
            _LOG(Logcxx::Level::ERRORS,"when transfer message couldn't find id,id:%d",id);
        }
        return true;
    }
}
