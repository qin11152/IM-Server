#include "AddFriendRequestJsonData.h"

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
    m_strVerifyMsg = m_ptree.get<bool>("VerifyMsg");
    m_strName = m_ptree.get<std::string>("Name");
    return;
}

std::string AddFriendRequestJsonData::generateJson()
{
    ptree m_ptree;

    m_ptree.put("MyId",m_strMyId.c_str());
    m_ptree.put("FriendId",m_strFriendId.c_str());
    m_ptree.put("VerifyMsg",m_strVerifyMsg);
    m_ptree.put("Name",m_strName);

    std::stringstream sstream;
    write_json(sstream,m_ptree);
    return sstream.str();
}