#include "HttpServer.h"
#include "PostHandle.h"

class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
	virtual Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &)
	{
		return new MyPostRequestHandler;
	}
};

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