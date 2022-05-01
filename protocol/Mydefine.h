#pragma once
#include <string>

class FriendInfo
{
public:
    std::string m_strFriendId{""};      //好友的id
    std::string m_strFriendName{""};     //好友的昵称
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
    FriendListRequest,
    FriendListResponse,
    GetFriendList,
    GetFriendListReply
};

struct LengthInfo
{
    int length;
    LengthInfo(int l):length(l){}
};

using MessageTypeBaseType=std::underlying_type<MessageType>;