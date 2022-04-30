#pragma once
#include <string>

class FriendInfo
{
public:
    std::string m_strFriendId{nullptr};      //好友的id
    std::string m_strFriendName{nullptr};     //好友的昵称
};

constexpr int PackageHeadSize=8;

enum class MessageType
{
    Default,
    RegisterRequest,
    RegisterResponse,
    LoginRequest,
    LoginResponse,
    InitialRequest,
    GroupChat,
    SingleChat,
    AddFriendRequest,
    AddFriendResponse,
    HeartPackage,
    GetFriendList,
    GetFriendListReply
};

struct LengthInfo
{
    int length;
    LengthInfo(int l):length(l){}
};

using MessageTypeBaseType=std::underlying_type<MessageType>;