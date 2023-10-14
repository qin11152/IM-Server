#include "GroupChatUserInfoTable.h"

namespace database
{
    GroupChatUserInfoTable::GroupChatUserInfoTable(std::string strGroupChatId):
    m_strGroupChatId(strGroupChatId)
    {}

    bool GroupChatUserInfoTable::createTable()
    {
        std::string query="create table group_chat_user_info_"+m_strGroupChatId+" (userId int NOT NULL,PRIMARY KEY (userId))";
        if(!DataBaseOperate::Instance()->execQuery(query.c_str(),nullptr))
        {
            _LOG(Logcxx::Level::ERRORS,"create table group_chat_user_info_%s failed,query is:%s",m_strGroupChatId.c_str(),query.c_str());
            return false;
        }
        return true;
    }

    bool GroupChatUserInfoTable::insertNewGroupChatUser(const std::vector<std::string> &vecUserId)
    {
        std::string query="";
        for(auto& userId:vecUserId)
        {
            query="insert into group_chat_user_info_"+m_strGroupChatId+" values("+userId+")";
            if(!DataBaseOperate::Instance()->execQuery(query.c_str(),nullptr))
            {
                _LOG(Logcxx::Level::ERRORS,"insert into group_chat_user_info_%s failed,query is:%s",m_strGroupChatId.c_str(),query.c_str());
                return false;
            }
        }
        return true;
    }
}