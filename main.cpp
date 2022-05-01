#include "ChatServer/ChatServer.h"
#include "MysqlQuery.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <thread>

void ThreadFunc(boost::asio::io_context& io)
{
    io.run();
}

int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        std::cout<<"usage: "<<argv[0]<<" port\n";
        return 1;
    }
    boost::asio::io_context io;
    unsigned short port=std::atoi(argv[1]);
    ChatServer server(io,port);
    
    io.run();
    return 0;
}