#pragma once

#include "JsonBaseData.h"

class AddFriendNotify:public JsonBaseData
{
public:
    AddFriendNotify(const std::string& message="");
    void parse(const std::string& message="")override;
    std::string generateJson()override;

    std::string m_strId1{ "" };
    std::string m_strId2{ "" };
    std::string m_strName1{""};
    std::string m_strName2{""};
    MessageType m_strType{ MessageType::AddFriendNotify };
};