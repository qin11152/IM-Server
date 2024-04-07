#pragma once

#include "DataBaseOperate.h"

namespace database
{
    class FriendInfoTable
    {
    public:
        FriendInfoTable()=default;
        ~FriendInfoTable()=default;
        FriendInfoTable(const FriendInfoTable&)=delete;
        FriendInfoTable& operator=(const FriendInfoTable&)=delete;

        /**
         * @brief Create a Table object
         * 
         * @return true 
         * @return false 
         */
        bool createTable();

        /**
         * @brief 将新成为好友的一对信息插入数据库
         * 
         * @param friendId_1 好友1的id
         * @param friendId_2 好友2的id
         * @param friend1name 好友1的name
         * @param friend2name 好友2的name
         * @return true 
         * @return false 
         */
        bool insertFriendInfo(std::string friendId_1,std::string friendId_2,std::string friend1name,std::string friend2name);

        /**
         * @brief 根据id获取此人的好友列表
         * 
         * @param vecFriendList 好友列表
         * @param strUserId 用户id
         * @return true 
         * @return false 
         */
        bool queryUserFrinedList(std::vector<FriendInfo>& vecFriendList,std::string& strUserId);

    private:
    };
}