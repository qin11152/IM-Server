/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2023-03-12 18:16:48
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-02 15:10:00
 * @FilePath: /src/IM-Server/protocol/StartGroupChatReplyJsonData.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "StartGroupChatReplyJsonData.h"

namespace protocol
{
	StartGroupChatReplyJsonData::StartGroupChatReplyJsonData(const std::string& message)
	{
		parse(message);
	}

	void StartGroupChatReplyJsonData::parse(const std::string& message)
	{
		if (message.empty())
		{
			return;
		}
		ptree m_ptree;
		std::stringstream sstream(message);
		read_json(sstream, m_ptree);
		m_strGroupId = m_ptree.get<std::string>("GroupId");
		m_strGuid = m_ptree.get<std::string>("Guid");
		m_bIsSuccess = m_ptree.get<bool>("IsSuccess");
		for (auto& item : m_ptree.get_child("GroupChatVecId"))
		{
			m_vecGroupChatId.push_back(item.second.get_value<std::string>());
		}
	}

	std::string StartGroupChatReplyJsonData::generateJson()
	{
		ptree m_ptree;
        m_ptree.put("type", static_cast<int>(m_strType));
        m_ptree.put("GroupId", m_strGroupId.c_str());
        m_ptree.put("Guid", m_strGuid.c_str());
        m_ptree.put("IsSuccess", m_bIsSuccess);
        ptree m_ptreeGroupChat;
        for (auto& item : m_vecGroupChatId)
        {
            m_ptreeGroupChat.push_back(std::make_pair("", ptree(item)));
        }
        m_ptree.add_child("GroupChatVecId", m_ptreeGroupChat);
        std::stringstream sstream;
        write_json(sstream, m_ptree);
        return sstream.str();
	}
}

