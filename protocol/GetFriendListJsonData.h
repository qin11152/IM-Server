#pragma once
#include "../JsonBaseData.h"

class GetFriendListJsonData:public JsonBaseData
{
public:
    GetFriendListJsonData(const std::string& message = "");
    virtual void parse(const std::string& message) override;
    virtual std::string generateJson() override;

    MessageType m_strType{ MessageType::GetFriendList };
    std::string m_strUserId{ "" };
};