/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2024-04-09 10:55:22
 * @FilePath: /IM-Server/main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "ChatServer/ChatServer.h"
#include "Log.h"
#include "PublicFunction.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <thread>

int main(int argc,char* argv[])
{
    Logcxx::get_mutable_instance().Init("./log4cxx.properties");
    database::DataBaseOperate::Instance();
    unsigned short port=19999;
    boost::asio::io_context io;
    net::ChatServer server(io,port);
    for(int i=0;i<10;++i)
    {
        std::thread t([&](){io.run();});
        t.detach();
    }
    io.run();
    return 0;
}
