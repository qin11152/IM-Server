#include "ChatServer/ChatServer.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

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
    unsigned short port=std::atoi(argv[1]);

    boost::asio::io_context io;
    ChatServer server(io,port);

    //boost::thread t(boost::bind(&boost::asio::io_context::run, &io));

    io.run();
    return 0;
}