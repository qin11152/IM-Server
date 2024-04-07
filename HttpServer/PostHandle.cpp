#include "PostHandle.h"

void MyPostRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
{
	if (req.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
	{
		std::string contentType = req.get("Content-Type", "");

		if (contentType.find("multipart/form-data") != std::string::npos)
		{
			FilePartHandler partHandler;
			HTMLForm form(req, req.stream(), partHandler);
			resp.setStatus(HTTPResponse::HTTP_OK);
			resp.setContentType("text/plain");
			std::ostream &out = resp.send();
			out << "Multipart form-data processed, file uploaded.";
		}
		else
		{
			// 打开一个文件用于保存上传的内容
			std::ofstream outputFile("uploaded_file", std::ios::binary);

			// 将请求的正文内容复制到文件中
			Poco::StreamCopier::copyStream(req.stream(), outputFile);

			// 设置响应
			resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
			resp.setContentType("text/plain");
			std::ostream &out = resp.send();
			out << "File uploaded successfully";
		}
	}
}