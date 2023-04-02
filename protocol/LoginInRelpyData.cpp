/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-02 15:07:41
 * @FilePath: /IM-Server/protocol/LoginInRelpyData.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "LoginInReplyData.h"

namespace protocol
{
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
} // namespace protocol