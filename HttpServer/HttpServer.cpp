/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2024-04-07 21:06:03
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2024-04-10 18:57:45
 * @FilePath: /IM-Server/HttpServer/HttpServer.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "HttpServer.h"
#include "PostHandle.h"

int MyServerApplication::main(const std::vector<std::string> &)
{
	{
		// 设置监听端口
		// 创建HTTP服务器参数
		Poco::Net::HTTPServerParams *pParams = new Poco::Net::HTTPServerParams;
		// 设置最大线程数
		pParams->setMaxThreads(50);

		ServerSocket svs(10068);
		// 创建HTTP服务器
		HTTPServer srv(new MyRequestHandlerFactory, svs, pParams);

		srv.start(); // 启动服务器
		// Application::instance().logger().information("HTTP Server started on port 8080.");

		// 等待服务器结束
		waitForTerminationRequest(); // 正确的使用方式

		srv.stop();

		return Application::EXIT_OK;
	}
}

Poco::Net::HTTPRequestHandler *MyRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &)
{
	return new MyPostRequestHandler;
}
