/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-02 15:07:00
 * @FilePath: /IM-Server/protocol/LoginInJsonData.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "LoginInJsonData.h"

namespace protocol
{
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
} // namespace protocol