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
	virtual void handlePart(const MessageHeader &header, std::istream &stream)
	{
		if (header.has("Content-Disposition"))
		{
			std::string disp;
			NameValueCollection params;
			MessageHeader::splitParameters(header["Content-Disposition"], disp, params);
			std::string filename = params.get("filename", "");
			if (!filename.empty())
			{
				std::ofstream ofs("uploaded_" + filename, std::ios::binary); // Prefix file name to avoid conflicts and direct path manipulation
				if (ofs.good())
				{
					Poco::StreamCopier::copyStream(stream, ofs);
				}
				else
				{
					std::cerr << "Error saving file." << std::endl;
				}
			}
		}
	}
};