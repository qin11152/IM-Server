#include "JsonBaseData.h"

class SingleChatMessageJsonData :public JsonBaseData
{
public:

    SingleChatMessageJsonData(const std::string& message = "");
    void parse(const std::string& message)override;
    std::string generateJson()override;

    //发送用户的id
    std::string m_strSendUserId{ "-1" };
    std::string m_strRecvUserId{ "-1" };
    std::string m_strMessage{ "" };
    std::string m_strTime{ "" };
    std::string m_strSendName{ "" };
    MessageType m_strType{ MessageType::SingleChat };
};