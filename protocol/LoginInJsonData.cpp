#include "LoginInJsonData.h"

LoginInJsonData::LoginInJsonData(const std::string& message)
{
    parse(message);
}

void LoginInJsonData::parse(const std::string& message)
{
    if(message.empty())
    {
        return;
    }
    ptree m_ptree;
    std::stringstream sstream(message);
    read_json(sstream,m_ptree);
    std::string userId = m_ptree.get<std::string>("UserId");
    m_strId = userId;
    std::string userpassword = m_ptree.get<std::string>("UserPassword");
    m_strPassword = userpassword;
    return;
}

std::string LoginInJsonData::generateJson()
{
    ptree m_ptree;
    m_ptree.put("type",static_cast<int>(m_strType));
    m_ptree.put("UserId",m_strId);
    m_ptree.put("UserPassword",m_strPassword);
    std::stringstream sstream;
    write_json(sstream,m_ptree);
    return sstream.str();
}