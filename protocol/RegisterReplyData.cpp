#include "RegisterReplyData.h"

RegisterReplyData::RegisterReplyData(const std::string& message)
{
    parse(message);
    m_strType=MessageType::RegisterResponse;
}

void RegisterReplyData::parse(const std::string& message)
{
    if(message.empty())
    {
        return;
    }
    ptree m_ptree;
    std::stringstream sstream(message);
    read_json(sstream,m_ptree);
    m_bRegisterResult=m_ptree.get<bool>("RegisterResult");
    m_iId=m_ptree.get<int>("UserId");
    return;
}

std::string RegisterReplyData::generateJson()
{
    ptree m_ptree;
    std::stringstream sstream;
    m_ptree.put("type",static_cast<int>(m_strType));
    m_ptree.put("RegisterResult",m_bRegisterResult);
    m_ptree.put("UserId",m_iId);
    write_json(sstream,m_ptree);
    return sstream.str();
}

