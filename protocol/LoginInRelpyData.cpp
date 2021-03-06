#include "LoginInReplyData.h"

LoginInReplyData::LoginInReplyData(const std::string&message)
{
    parse(message);
}

void LoginInReplyData::parse(const std::string& message)
{
    if (message.empty())
    {
        return;
    }
    ptree m_ptree;
    std::stringstream sstream(message);
    read_json(sstream, m_ptree);
    bool loginInResult = m_ptree.get<bool>("LoginInResult");
    std::string m_strUserName=m_ptree.get<std::string>("Name");
    m_bLoginInResult = loginInResult;
    return;
}

std::string LoginInReplyData::generateJson()
{
    ptree m_ptree;
    m_ptree.put("type",static_cast<int>(m_strType));
    m_ptree.put("LoginInResult",m_bLoginInResult);
    m_ptree.put("Name",m_strUserName);
    std::stringstream sstream;
    write_json(sstream,m_ptree);
    return sstream.str();
}