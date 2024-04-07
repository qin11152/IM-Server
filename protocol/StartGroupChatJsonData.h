/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2023-10-17 21:34:25
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-10-17 21:34:48
 * @FilePath: /IM-Server/protocol/StartGroupChatJsonData.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "JsonBaseData.h"

namespace protocol
{
	class StartGroupJsonData : public JsonBaseData
	{
	public:
		StartGroupJsonData(const std::string& message = "");
		~StartGroupJsonData();
		void parse(const std::string& message)override;
		std::string generateJson()override;

		std::string m_strStarterId{ "" };
		std::string m_strGroupName{ "" };
		std::string m_strImagePathInServer{ "" };
		std::vector<std::string> m_vecGroupChat;
		MessageType m_strType{ MessageType::StartGroupChat };
	};
}