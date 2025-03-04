#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Application.h>

using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Util::ServerApplication;
using Poco::Util::Application;

class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
	virtual Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &);
};

class MyServerApplication: public ServerApplication 
{
protected:
    int main(const std::vector<std::string>&);
};
