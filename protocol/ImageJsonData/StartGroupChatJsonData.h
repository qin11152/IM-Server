/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2023-03-24 20:30:36
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-05 16:59:30
 * @FilePath: /IM-Server/protocol/ImageJsonData/StartGroupChatJsonData.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "protocol/ImageJsonData/ImageJsonDataBase.h"


namespace protocol
{
    namespace image
    {
        class StartGroupChatInnerData : public ImageInnerJsonData
        {
        public:
            StartGroupChatInnerData(const std::string& message = "");
            ~StartGroupChatInnerData()override;
            void parse(const std::string& message)override;
            std::string generateJson()override;

            std::string m_strStarterId{ "" };
            std::string m_strGuid{ "" };
            std::string m_strGroupName{ "" };
            std::vector<std::string> m_vecGroupChat;

            MessageType m_strType{ MessageType::StartGroupChat };

        };

        class StartGroupJsonData : public ImageJsonBaseData
        {
        public:
            StartGroupJsonData(const std::string& message = "");
            ~StartGroupJsonData()override;
            void parse(const std::string& message)override;
            std::string generateJson()override;

            StartGroupChatInnerData m_stInnerData;   //内部可包含一层数据，可为空
        };
    }
}
