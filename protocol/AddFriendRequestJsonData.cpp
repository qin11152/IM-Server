/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-02 14:58:48
 * @FilePath: /IM-Server/protocol/AddFriendRequestJsonData.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "AddFriendRequestJsonData.h"

namespace protocol
{
    AddFriendRequestJsonData::AddFriendRequestJsonData(const std::string& message/*=""*/)
    {
        parse(message);
    }

    void AddFriendRequestJsonData::parse(const std::string& message/*=""*/)
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
        m_strVerifyMsg = m_ptree.get<std::string>("VerifyMsg");
        m_strName = m_ptree.get<std::string>("Name");
        return;
    }

    std::string AddFriendRequestJsonData::generateJson()
    {
        StringBuffer strbuf;
        Writer<rapidjson::StringBuffer> writer(strbuf);

        writer.StartObject();

        writer.Key("type");
        writer.Int(static_cast<int>(m_strType));

        writer.Key("MyId");
        writer.String(m_strMyId.c_str());

        writer.Key("FriendId");
        writer.String(m_strFriendId.c_str());

        writer.Key("VerifyMsg");
        writer.String(m_strVerifyMsg.c_str());

        writer.Key("Name");
        writer.String(m_strName.c_str());

        writer.EndObject();

        return strbuf.GetString();
    }
} // namespace protocol