#pragma once
#include "JsonBaseData.h"

namespace protocol
{
    class GetFriendListJsonData:public JsonBaseData
    {
    public:
        GetFriendListJsonData(const std::string& message = "");
        virtual void parse(const std::string& message) override;
        virtual std::string generateJson() override;

        MessageType m_strType{ MessageType::GetFriendListRequest };
        std::string m_strUserId{ "" };
    };
} // namespace protocol