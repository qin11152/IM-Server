#pragma once

#include "JsonBaseData.h"

class AddFriendRequestJsonData:public JsonBaseData
{
public:
    AddFriendRequestJsonData(const std::string& message="");
    void parse(const std::string& message="")override;
    std::string generateJson()override;

    std::string m_strMyId{ "" };
    std::string m_strFriendId{ "" };
    std::string m_strVerifyMsg{ "" };
    MessageType m_strType{ MessageType::AddFriendRequest };
};