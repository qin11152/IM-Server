#include "ChatClient.h"
#include <unordered_map>

class ChatServer
{
public:
    ChatServer(io_context& io_ctx,short port);
    void DoAccept();

private:
    tcp::acceptor m_acceptor;
    static std::unordered_map<int,std::shared_ptr<ChatClient>> m_userMap;
};