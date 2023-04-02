/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-02 15:02:59
 * @FilePath: /IM-Server/protocol/AddFriendResponseJsonData.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "AddFriendResponseJsonData.h"

namespace protocol
{
    AddFriendResponseJsonData::AddFriendResponseJsonData(const std::string& message /*= ""*/)
    {
        parse(message);
    }

    void AddFriendResponseJsonData::parse(const std::string& message)
    {
        if (message.empty())
        {
            return;
        }
        ptree m_ptree;
        std::stringstream sstream(message);
        read_json(sstream, m_ptree);
        m_strMyId = m_ptree.get<std::string>("MyId");
        m_strFriendId = m_ptree.get<std::string>("FriendId");
        m_bResult = m_ptree.get<bool>("AddFriendResult");
        return;
    }

    std::string AddFriendResponseJsonData::generateJson()
    {
        ptree m_ptree;

        m_ptree.put("type",static_cast<int>(m_strType));
        m_ptree.put("MyId",m_strMyId.c_str());
        m_ptree.put("FriendId",m_strFriendId.c_str());
        m_ptree.put("AddFriendResult",m_bResult);

        std::stringstream sstream;
        write_json(sstream,m_ptree);
        return sstream.str();
    }
} // namespace protocol
