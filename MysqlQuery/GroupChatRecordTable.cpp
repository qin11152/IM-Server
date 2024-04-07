/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2023-10-14 16:37:25
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-10-17 21:29:07
 * @FilePath: /IM-Server/MysqlQuery/GroupChatRecordTable.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "GroupChatRecordTable.h"

namespace database
{
    GroupChatRecordTable::GroupChatRecordTable(std::string strGroupChatId):
    m_strGroupChatId(strGroupChatId)
    {
    }

    bool GroupChatRecordTable::createTable()
    {
        std::string query="create table group_chat_record_"+m_strGroupChatId+" (userid varchar(255),message varchar(255),username varchar(255),sendtime varchar(30))";
        if(!DataBaseOperate::Instance()->execQuery(query.c_str(),nullptr))
        {
            _LOG(Logcxx::Level::ERRORS,"create table group_chat_user_info_%s failed,query is:%s",m_strGroupChatId.c_str(),query.c_str());
            return false;
        }
        return true;
    }
}