/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-10-17 20:59:15
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

struct PackageHead
{
    char flagBit[4]{ "&q*" };
    unsigned int length;
    unsigned char cmdId;
};
constexpr int PackageHeadSize=sizeof(PackageHead);
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
	StartGroupChatReply,
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
	GetFriendListRequest,
	GetFriendListReply,
	ProfileImageMsg,
	getFriendProfileImage,
	getFriendProfileImageReply,
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

struct MyRecvImageInfo
{
    std::string m_strImageName{""};
    std::string m_strJsonMsg{""};
    int m_nImageSize{0};
};
