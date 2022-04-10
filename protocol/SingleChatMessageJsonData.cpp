#include "SingleChatMessageJsonData.h"

SingleChatMessageJsonData::SingleChatMessageJsonData(const std::string& message /*= ""*/)
{
    parse(message);
}

void SingleChatMessageJsonData::parse(const std::string& message)
{
    if (message.empty())
    {
        return;
    }
    ptree m_ptree;
    std::stringstream sstream;
    read_json(sstream, m_ptree);
    m_strSendUserId = m_ptree.get<std::string>("SendUserId");
    m_strRecvUserId = m_ptree.get<std::string>("RecvUserId");
    m_strMessage = m_ptree.get<std::string>("Message");
}

std::string SingleChatMessageJsonData::generateJson()
{

    ptree m_ptree;
    m_ptree.put("type",m_strType);
    m_ptree.put("SendUserId",m_strSendUserId.c_str());
    m_ptree.put("RecvUserId",m_strRecvUserId.c_str());
    m_ptree.put("Message",m_strMessage.c_str());
    
    std::stringstream sstream;
    write_json(sstream,m_ptree);
    return sstream.str();

}