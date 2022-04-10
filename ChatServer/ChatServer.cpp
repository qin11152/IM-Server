#include "ChatServer.h"
#include <iostream>

ChatServer::ChatServer(io_context& io_ctx,short port)
:m_ioc(io_ctx),
m_acceptor(io_ctx,tcp::endpoint(tcp::v4(),port))
{
    DoAccept();
}

void ChatServer::DoAccept()
{
    m_acceptor.async_accept(
        [this](std::error_code ec,tcp::socket socket){
            if(!ec)
            {
                auto new_Acceptor=std::make_shared<ChatClient>(std::move(socket),this,m_ioc);
                m_socketVec.push_back(new_Acceptor);
                new_Acceptor->Start();
            }
            DoAccept();
        }
    );
}

void ChatServer::removeDisconnetedClient(int id,std::shared_ptr<ChatClient> ptr)
{
    if(m_idMap.find(id)!=m_idMap.end())
    {
        m_idMap.erase(id);
    }
    //printf("before size is:%d\n",m_socketVec.size());
    for(auto iter=m_socketVec.begin();iter!=m_socketVec.end();++iter)
    {
        if(*iter==ptr)
        {
            //std::cout<<(*iter).use_count()<<std::endl;
            m_socketVec.erase(iter);
            return;
        }
    }
}

void ChatServer::insertIntoIdMap(int id,std::shared_ptr<ChatClient> m_clientPtr)
{
    m_idMap[id]=m_clientPtr;
}

bool ChatServer::transferMessage(int id,std::string& message)
{
    if(m_idMap.find(id)!=m_idMap.end())
    {
        m_idMap[id]->DoWrite(message,message.length());
    }
}
