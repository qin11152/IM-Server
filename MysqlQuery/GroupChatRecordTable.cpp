#include "GroupChatRecordTable.h"

namespace database
{
    GroupChatUserInfoTable(std::string strGroupChatId):
    m_strGroupChatId(strGroupChatId)
    {}

    bool GroupChatUserInfoTable::createTable()
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