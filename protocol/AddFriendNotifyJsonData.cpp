/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-02 15:11:29
 * @FilePath: /src/IM-Server/protocol/AddFriendNotifyJsonData.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "AddFriendNotifyJsonData.h"

namespace protocol
{
    AddFriendNotify::AddFriendNotify(const std::string& message)
    {
        parse(message);
    }

    void AddFriendNotify::parse(const std::string& message)
    {
        if (message.empty())
        {
            return;
        }
        ptree m_ptree;
        std::stringstream sstream(message);
        read_json(sstream, m_ptree);
        m_strId1 = m_ptree.get<std::string>("Id1");
        m_strId2 = m_ptree.get<std::string>("Id2");
        m_strName1=m_ptree.get<std::string>("Name1");
        m_strName2=m_ptree.get<std::string>("Name2");
        m_strImageStamp1=m_ptree.get<std::string>("TimeStamp1");
        m_strImageStamp2=m_ptree.get<std::string>("TimeStamp2");
        return;
    }

    std::string AddFriendNotify::generateJson()
    {
        ptree m_ptree;

        m_ptree.put("type",static_cast<int>(m_strType));
        m_ptree.put("Id1",m_strId1.c_str());
        m_ptree.put("Id2",m_strId2.c_str());
        m_ptree.put("Name1",m_strName1.c_str());
        m_ptree.put("Name2",m_strName2.c_str());
        m_ptree.put("TimeStamp1",m_strImageStamp1.c_str());
        m_ptree.put("TimeStamp2",m_strImageStamp2.c_str());
        
        std::stringstream sstream;
        write_json(sstream,m_ptree);
        return sstream.str();
    }
} // namespace protocol
