#pragma once

#include "DataBaseOperate.h"

namespace database
{
    class GroupChatInfoTable
    {
    public:
        GroupChatInfoTable()=default;
        ~GroupChatInfoTable()=default;
        GroupChatInfoTable(const GroupChatInfoTable&)=delete;
        GroupChatInfoTable& operator=(const GroupChatInfoTable&)=delete;

        /**
         * @brief Create a Table object
         * 
         * @return true 
         * @return false 
         */
        bool createTable();

        /**
         * @brief Get the Cur Group Id object
         * 
         * @param groupId 返回的群聊id
         * @return true 
         * @return false 
         */
        bool getCurGroupId(std::string& groupId);

        /**
         * @brief 插入一个新的群聊的相关信息
         * 
         * @param groupId 群聊id
         * @param groupName 群聊名称
         * @param groupIconPath 群聊头像路径
         * @return true 
         * @return false 
         */
        bool insertGroupInfo(const std::string& groupId,const std::string& groupName,const std::string& groupIconPath);
    };
}