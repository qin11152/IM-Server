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
    AddFriendNotify,
    HeartPackage,
    FriendListRequest,
    FriendListResponse,
    RefreshFriendListRequest,
    RefreshFriendListResponse,
    GetFriendList,
    GetFriendListReply,
    ProfileImageMsg,
};

struct LengthInfo
{
    int length;
    LengthInfo(int l):length(l){}
};

using MessageTypeBaseType=std::underlying_type<MessageType>;

struct MyAddFriendInfo
{
    std::string m_strMyId{""};
    std::string m_strFriendId{""};
    std::string m_strVerifyMsg{""};
    MyAddFriendInfo(std::string myId="",std::string friendId="",std::string verifyMsg="")
    :m_strMyId(myId),
    m_strFriendId(friendId),
    m_strVerifyMsg(verifyMsg)
    {}
};

struct MyChatMessageInfo
{
    std::string m_strFromId{""};
    std::string m_strToId{""};
    std::string m_strChatMsg{""};
    std::string m_strSendName{""};
    MyChatMessageInfo(std::string fromId="",std::string toId="",std::string chatMsg="",std::string sendName="")
    :m_strFromId(fromId),
    m_strToId(toId),
    m_strChatMsg(chatMsg),
    m_strSendName(sendName)
    {}
};

