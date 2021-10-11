#include "ChatServer.h"

ChatServer::ChatServer(io_context& io_ctx,short port)
:m_acceptor(io_ctx,tcp::endpoint(tcp::v4(),port))
{
    DoAccept();
}

void ChatServer::DoAccept()
{
    m_acceptor.async_accept(
        [this](std::error_code ec,tcp::socket socket){
            if(!ec)
            {
                auto new_Acceptor=std::make_shared<ChatClient>(std::move(socket));
                new_Acceptor->Start();
            }
            DoAccept();
        }
    );
}