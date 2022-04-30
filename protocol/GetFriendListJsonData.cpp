#include "GetFriendListJsonData.h"

GetFriendListJsonData::GetFriendListJsonData(const std::string& message /*= ""*/)
{
    parse(message);
}

void GetFriendListJsonData::parse(const std::string& message)
{
    if (message.empty())
    {
        return;
    }
    ptree m_ptree;
    std::stringstream sstream(message);
    read_json(sstream, m_ptree);
    m_strUserId=m_ptree.get<std::string>("UserId");
    return;
}

std::string GetFriendListJsonData::generateJson()
{
    ptree m_ptree;
    m_ptree.put("type",static_cast<int>(m_strType));
    m_ptree.put("UserId",m_strUserId.c_str());

    std::stringstream sstream;
    write_json(sstream,m_ptree);
    return sstream.str();
}