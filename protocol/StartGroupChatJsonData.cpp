/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2023-10-17 21:34:31
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-10-17 21:36:48
 * @FilePath: /IM-Server/protocol/StartGroupChatJsonData.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "StartGroupChatJsonData.h"

namespace protocol
{

	StartGroupJsonData::StartGroupJsonData(const std::string& message)
	{
		parse(message);
	}

	StartGroupJsonData::~StartGroupJsonData()
	{
	}

	void StartGroupJsonData::parse(const std::string& message)
	{
		if (message.empty())
		{
			return;
		}
		ptree m_ptree;
		std::stringstream sstream(message);
		read_json(sstream, m_ptree);
		m_strGroupName = m_ptree.get<int>("GroupName");
		m_strImagePathInServer = m_ptree.get<std::string>("ImagePath");
		m_strStarterId = m_ptree.get<std::string>("StartId");
		//获取json对象中id数组
		for (auto& item : m_ptree.get_child("GroupChatId"))
		{
			m_vecGroupChat.push_back(item.second.get_value<std::string>());
		}
	}

	std::string StartGroupJsonData::generateJson()
	{
		//rapisjson生成json
		ptree m_ptree;
        m_ptree.put("type", static_cast<int>(m_strType));
        m_ptree.put("GroupName", m_strGroupName.c_str());
        m_ptree.put("ImagePath", m_strImagePathInServer.c_str());
        m_ptree.put("StartId", m_strStarterId.c_str());
        ptree m_ptreeGroupChat;
        for (auto& item : m_vecGroupChat)
        {
            m_ptreeGroupChat.push_back(std::make_pair("", ptree(item)));
        }
        m_ptree.add_child("GroupChatId", m_ptreeGroupChat);

        std::stringstream sstream;
        write_json(sstream, m_ptree);
        return sstream.str();
	}
}