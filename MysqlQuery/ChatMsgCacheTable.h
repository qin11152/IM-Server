#pragma once

#include "DataBaseOperate.h"

namespace database
{
    class ChatMsgCacheTable
    {
    public:
        ChatMsgCacheTable()=default;
        ~ChatMsgCacheTable()=default;
        ChatMsgCacheTable(const ChatMsgCacheTable&)=delete;
        ChatMsgCacheTable& operator=(const ChatMsgCacheTable&)=delete;

        /**
         * @brief Create a Table object
         * 
         * @return true 
         * @return false 
         */
        bool createTable();

        /**
         * @brief 根据id获取自己不在线时的缓存小徐
         * 
         * @param vecFriednInfo 聊天消息
         * @param id 目标用户id
         * @return true 
         * @return false 
         */
        bool queryCachedChatMsg(std::vector<MyChatMessageInfo>& vecFriednInfo,std::string& id);

        /**
         * @brief 聊天对象不在线的时候，将聊天消息缓存到数据库
         * 
         * @param fromId 发送者id
         * @param toId 接受者id
         * @param msg 聊天消息
         * @param sendName 发送者昵称
         * @param time 消息时间
         * @return true 
         * @return false 
         */
        bool insertCachedChatMsg(std::string& fromId,std::string& toId,std::string& msg,std::string& sendName,std::string& time);

        /**
         * @brief 删除用户id对应的缓存聊天消息
         * 
         * @param id 用户id
         * @return true 
         * @return false 
         */
        bool deleteCachedChatMsg(std::string& id);

    };
    
    
}