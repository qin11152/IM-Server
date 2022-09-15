/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2022-09-07 21:47:46
 * @FilePath: /src/IM-Server/protocol/Mydefine.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include <string>

class FriendInfo
{
public:
    std::string m_strFriendId{""};      //好友的id
    std::string m_strFriendName{""};     //好友的昵称
    std::string m_strImageTimeStamp{""};    //好友的头像时间戳
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

