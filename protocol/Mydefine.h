/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-02-21 21:58:39
 * @FilePath: /src/IM-Server/protocol/Mydefine.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <string>

class FriendInfo
{
public:
    std::string m_strFriendId{""};      //好友的id
    std::string m_strFriendName{""};     //好友的昵称
    std::string m_strImageTimeStamp{""};    //好友的头像时间戳
};

constexpr int PackageHeadSize=8;
constexpr int kSegmentLength = 9000;

/**
 * brief：根据不同的需求来传递头像，做出不同处理.
 */
enum class ProfileImageType
{
    UpdateProfileImage=0,       //要求更新头像
    AddFriendProfileImage,      //添加好友时传送的头像
};

enum class MessageType
{
Default,
    RegisterRequest,
    RegisterResponse,
    LoginRequest,
    LoginResponse,
    InitialRequest,
    StartGroupChat,
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
    getFriendProfileImage,
    getFriendProfileImageReply,
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

