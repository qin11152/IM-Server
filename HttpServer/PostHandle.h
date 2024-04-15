/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2024-04-07 21:06:03
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2024-04-15 19:06:36
 * @FilePath: /IM-Server/HttpServer/PostHandle.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/PartHandler.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Application.h>
#include <Poco/StreamCopier.h>
#include <fstream>
#include <filesystem>
#include <iostream>

using namespace Poco::Net;
using namespace Poco::Util;

class MyPostRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
	virtual void handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp) override;
};

class FilePartHandler : public PartHandler
{
public:
	virtual void handlePart(const MessageHeader &header, std::istream &stream);

	std::string getSavePath()const;

private:
	std::string m_strFileSavePath{""};
};