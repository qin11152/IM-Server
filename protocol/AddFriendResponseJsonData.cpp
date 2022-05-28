#include "AddFriendResponseJsonData.h"

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

    m_ptree.put("MyId",m_strMyId.c_str());
    m_ptree.put("FriendId",m_strFriendId.c_str());
    m_ptree.put("AddFriendResult",m_bResult);

    std::stringstream sstream;
    write_json(sstream,m_ptree);
    return sstream.str();
}
