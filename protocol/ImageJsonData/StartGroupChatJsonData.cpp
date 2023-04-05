/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2023-03-24 20:30:43
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-05 18:34:00
 * @FilePath: /IM-Server/protocol/ImageJsonData/StartGroupChatJsonData.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "StartGroupChatJsonData.h"

namespace protocol
{
    namespace image
    {
        StartGroupChatInnerData::StartGroupChatInnerData(const std::string& message)
        {
            parse(message);
        }

        StartGroupChatInnerData::~StartGroupChatInnerData()
        {
        }

        void StartGroupChatInnerData::parse(const std::string& message)
        {
            if (message.empty())
            {
                return;
            }

            //rapidsjon解析
            Document document;
            document.Parse(message.c_str());
            if (document.HasParseError())
            {
                printf("GetParseError %d\n",document.GetParseError());
                return;
            }
            if (document.HasMember("StarterId"))
            {
                m_strStarterId = document["StarterId"].GetString();
            }
            if (document.HasMember("Guid"))
            {
                m_strGuid = document["Guid"].GetString();
            }
            if (document.HasMember("GroupName"))
            {
                m_strGroupName = document["GroupName"].GetString();
            }
            if (document.HasMember("GroupChatId"))
            {
                const Value& array = document["GroupChatId"];
                for (SizeType i = 0; i < array.Size(); i++)
                {
                    m_vecGroupChat.push_back(array[i].GetString());
                }
            }
            
        }

        std::string StartGroupChatInnerData::generateJson()
        {
            ptree m_ptree;
            m_ptree.put("type", static_cast<int>(m_strType));
            m_ptree.put("StarterId", m_strStarterId.c_str());
            m_ptree.put("GroupName", m_strGroupName.c_str());
            m_ptree.put("Guid", m_strGuid.c_str());
            ptree m_ptreeGroupChat;
            for (auto& item : m_vecGroupChat)
            {
                m_ptreeGroupChat.push_back(std::make_pair("", ptree(item)));
            }
            m_ptree.add_child("GroupChatId", m_ptreeGroupChat);

            std::stringstream sstream;
            write_json(sstream, m_ptree);
            return sstream.str();
        }

        StartGroupJsonData::StartGroupJsonData(const std::string& message)
        {
            parse(message);
        }

        StartGroupJsonData::~StartGroupJsonData()
        {
        }

        void StartGroupJsonData::parse(const std::string& message)
        {

            //使用rapidjson解析
            Document document;
            document.Parse(message.c_str());
            if (document.HasParseError())
            {
                printf("GetParseError %d\n",document.GetParseError());
                return;
            }
            if (document.HasMember("ImageLength"))
            {
                m_iImageLenth = document["ImageLength"].GetInt();
            }
            if (document.HasMember("ImageSuffix"))
            {
                m_strImageSuffix = document["ImageSuffix"].GetString();
            }
            if (document.HasMember("InnerData"))
            {
                m_stInnerData.parse(document["InnerData"].GetString());
            }
        }

        std::string StartGroupJsonData::generateJson()
        {
            ptree m_ptree;
            m_ptree.put("type", static_cast<int>(m_strType));
            m_ptree.put("ImageLength", m_iImageLenth);
            m_ptree.put("ImageSuffix", m_strImageSuffix.c_str());
            m_ptree.put("InnerData", m_stInnerData.generateJson().c_str());
            std::stringstream sstream;
            write_json(sstream, m_ptree);
            return sstream.str();
        }
    }
}