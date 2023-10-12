#pragma once

#include "DataBaseOperate.h"

namespace database
{
    class AddFriendCacheTable
    {
    public:
        AddFriendCacheTable()=default;
        ~AddFriendCacheTable()=default;
        AddFriendCacheTable(const AddFriendCacheTable&)=delete;
        AddFriendCacheTable& operator=(const AddFriendCacheTable&)=delete;

        /**
         * @brief Create a Table object
         * 
         * @return true 
         * @return false 
         */
        bool createTable();

        /**
         * @brief 好友不在线时把给他的好友请求缓存起来
         * 
         * @param requestId 请求添加好友人的id
         * @param destinationId 被请求人的id
         * @param verifyMsg 添加好友时的验证消息
         * @return true 
         * @return false 
         */
        bool insertAddFriendCache(const std::string& requestId,const std::string& destinationId,const std::string& verifyMsg);

        /**
         * @brief 根据id获取服务器中缓存的好友请求
         * 
         * @param vecFriednInfo 好友请求vec
         * @param id 用户id
         * @return true 
         * @return false 
         */
        bool queryCachedAddFriendInfo(std::vector<MyAddFriendInfo>& vecFriednInfo,std::string& id);

        /**
         * @brief 把此id缓存的好友请求删除
         * 
         * @param id 用户id
         * @return true 
         * @return false 
         */
        bool deleteCachedAddFriendInfo(std::string& id);

    private:
    };
}