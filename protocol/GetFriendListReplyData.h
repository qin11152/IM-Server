#pragma once
#include "JsonBaseData.h"

class GetFriendListReplyData:public JsonBaseData
{
public:
    GetFriendListReplyData(const std::string& message = "");
    virtual void parse(const std::string& message) override;
    virtual std::string generateJson() override;

    MessageType m_strType{ MessageType::GetFriendListReply };
    std::vector<FriendInfo> m_vecFriendList;
};