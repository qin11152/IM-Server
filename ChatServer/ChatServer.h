#include "ChatClient.h"
#include <unordered_map>
#include <vector>

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
    std::vector<std::shared_ptr<ChatClient>> m_socketVec;   //存储的是连接上来的socket，不一定有id，可能是注册的时候连接上来的
    std::unordered_map<int,std::shared_ptr<ChatClient>> m_idMap;//每个socket对应的id，也就是对应的用户id
    boost::asio::io_context& m_ioc;
};