#pragma once
#include "JsonBaseData.h"

class LoginInReplyData
    :public JsonBaseData
{
public:
    LoginInReplyData(const std::string& message = "");
    void parse(const std::string& message)override;
    std::string generateJson()override;

    bool m_bLoginInResult{ false };
    std::string m_strUserName{""};
    MessageType m_strType{ MessageType::LoginResponse };
};
