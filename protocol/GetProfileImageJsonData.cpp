/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-11-27 16:46:02
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-02 15:04:16
 * @FilePath: /src/IM-Server/protocol/GetProfileImageJsonData.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "GetProfileImageJsonData.h"

namespace protocol
{
    GetProfileImageJsonData::GetProfileImageJsonData(const std::string& message)
    {
        parse(message);
    }

    void GetProfileImageJsonData::parse(const std::string& message)
    {
        if (message.empty())
        {
            return;
        }
        ptree m_ptree;
        std::stringstream sstream(message);
        read_json(sstream, m_ptree);
        m_strId = m_ptree.get<std::string>("UserId");
        return;
    }

    std::string GetProfileImageJsonData::generateJson()
    {
        ptree m_ptree;

        m_ptree.put("type",static_cast<int>(m_strType));
        m_ptree.put("UserId",m_strId.c_str());
        
        std::stringstream sstream;
        write_json(sstream,m_ptree);
        return sstream.str();
    }
} // namespace protocol
