/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2024-04-07 21:06:03
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2024-04-08 14:44:25
 * @FilePath: /IM-Server/HttpServer/PostHandle.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "PostHandle.h"

void MyPostRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
{
	printf("request\n");
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
			printf("recv\n");
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