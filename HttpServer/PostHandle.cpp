/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2024-04-07 21:06:03
 * @LastEditors: qin11152 1052080761@qq.com
 * @FilePath: /IM-Server/HttpServer/PostHandle.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "PostHandle.h"
#include <filesystem>

void MyPostRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
{
	printf("request\n");
	if (req.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
	{
		printf("post request\n");
		std::string contentType = req.get("Content-Type", "");

		if (contentType.find("multipart/form-data") != std::string::npos)
		{
			FilePartHandler partHandler;
			HTMLForm form(req, req.stream(), partHandler);
			resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK); // Fix: Replace HTTPResponse with Poco::Net::HTTPResponse
			resp.setContentType("text/plain");
			std::ostream &out = resp.send();
			out << partHandler.getSavePath();
		}
		else
		{
			// 打开一个文件用于保存上传的内容
			std::ofstream outputFile("uploaded_file", std::ios::binary);

			// 将请求的正文内容复制到文件中
			Poco::StreamCopier::copyStream(req.stream(), outputFile);

			// 设置响应
			resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK); // Fix: Replace HTTPResponse with Poco::Net::HTTPResponse
			resp.setContentType("text/plain");
			std::ostream &out = resp.send();
			out << "File uploaded successfully";
		}
	}
	else if (req.getMethod() == Poco::Net::HTTPRequest::HTTP_GET)
	{
		std::string filename = req.getURI().substr(1); // 假设URI的格式为 "/filename"
		printf("filename: %s\n", filename.c_str());
		std::ifstream file(filename, std::ios::binary);
		if (file)
		{
			auto size = std::filesystem::file_size(filename);
			resp.setContentLength(size); // 设置Content-Length头部
			resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK); // Fix: Replace HTTPResponse with Poco::Net::HTTPResponse
			resp.setContentType("application/octet-stream"); // 或者根据文件类型设置合适的MIME类型
			std::ostream &out = resp.send();
			out << file.rdbuf();
		}
		else
		{
			resp.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND); // Fix: Replace HTTPResponse with Poco::Net::HTTPResponse
			resp.send() << "File not found";
		}
	}
	else
	{
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED); // Fix: Replace HTTPResponse with Poco::Net::HTTPResponse
		resp.setContentType("text/plain");
		std::ostream &out = resp.send();
		out << "invalid requests are allowed";
	}
}

void FilePartHandler::handlePart(const MessageHeader &header, std::istream &stream)
{
	{
		if (header.has("Content-Disposition"))
		{
			std::string disp;
			NameValueCollection params;
			MessageHeader::splitParameters(header["Content-Disposition"], disp, params);
			std::string filename = params.get("filename", "");
			std::string name = params.get("name", "");
			if (!filename.empty())
			{
				printf("handle file");
				if ("file" == name)
				{
					std::string id=params.get("id","");
					//查看uploadfile文件夹是否存在
					std::string path = "uploadfile";
					if (!std::filesystem::exists(path))
					{
						std::filesystem::create_directory(path);
					}
					//查看id文件夹是否存在
					path = path + "/" + id;
					if (!std::filesystem::exists(path))
					{
						std::filesystem::create_directory(path);
					}
					//使用系统接口，获取年和月，查看/年/月文件夹是否存在
					time_t now = time(0);
					tm *ltm = localtime(&now);
					path = path + "/" + std::to_string(1900 + ltm->tm_year);
					if (!std::filesystem::exists(path))
					{
						std::filesystem::create_directory(path);
					}
					path = path + "/" + std::to_string(1 + ltm->tm_mon);
					if (!std::filesystem::exists(path))
					{
						std::filesystem::create_directory(path);
					}
					//保存在/年/月文件夹下
					path = path + "/" + filename;
					std::ofstream ofs(path, std::ios::binary); // Prefix file name to avoid conflicts and direct path manipulation
					if (ofs.good())
					{

						Poco::StreamCopier::copyStream(stream, ofs);
						m_strFileSavePath=path;
					}
					else
					{
						printf("error saving\n");
						std::cerr << "Error saving file." << std::endl;
					}
				}
			}
			else if (!name.empty())
			{
				if ("description" == name)
				{
					// TODO解析协议
				}
			}
		}
	}
}

std::string FilePartHandler::getSavePath() const
{
	return m_strFileSavePath;
}
