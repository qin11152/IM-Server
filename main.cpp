#include "ChatServer/ChatServer.h"
#include "MysqlQuery.h"
#include "Log.h"
#include "PublicFunction.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <thread>
#include <unistd.h>

int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        std::cout<<"usage: "<<argv[0]<<" port\n";
        return 1;
    }
    Logcxx::get_mutable_instance().Init("./log4cxx.properties");
    unsigned short port=std::atoi(argv[1]);
    boost::asio::io_context io;
    ChatServer server(io,port);
    for(int i=0;i<5;++i)
    {
        std::thread t([&](){io.run();});
        t.detach();
    }
    io.run();
    return 0;
}