/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2023-10-14 15:18:07
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-10-14 16:11:07
 * @FilePath: /IM-Server/MysqlQuery/GroupChatUserInfoTable.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "DataBaseOperate.h"

#include <vector>

namespace database
{
    class GroupChatUserInfoTable
    {
    public:
        GroupChatUserInfoTable(std::string strGroupChatId);
        ~GroupChatUserInfoTable()=delete;
        const GroupChatUserInfoTable& operator=(const GroupChatUserInfoTable& other)=delete;
        GroupChatUserInfoTable(const GroupChatUserInfoTable& other)=delete;

        /**
         * @brief 创建表
         * 
         * @return true 
         * @return false 
         */
        bool createTable();

        /**
         * @brief 将新的群聊用户插入到表中
         * 
         * @param vecUserId 新用户id
         * @return true 
         * @return false 
         */
        bool insertNewGroupChatUser(const std::vector<std::string> &vecUserId);

    private:
        std::string m_strGroupChatId{""};
    };
}