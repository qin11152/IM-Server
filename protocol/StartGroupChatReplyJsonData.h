/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2023-03-12 18:16:44
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-03-12 19:07:23
 * @FilePath: /src/IM-Server/protocol/StartGroupChatReplyJsonData.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "JsonBaseData.h"

namespace protocol
{
    class StartGroupChatReplyJsonData :public JsonBaseData
    {
    public:
        explicit StartGroupChatReplyJsonData(const std::string& message = "");
        void parse(const std::string& message)override;
        std::string generateJson()override;

        std::string m_strGroupId{ "" };
        std::string m_strGuid{ "" };
        std::vector<std::string> m_vecGroupChatId;
        bool m_bIsSuccess{ false };

        MessageType m_strType{ MessageType::StartGroupChatReply };
    };
}