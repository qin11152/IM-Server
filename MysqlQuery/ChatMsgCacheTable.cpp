#include "ChatMsgCacheTable.h"
#include "module/Log.h"

namespace database
{
    bool ChatMsgCacheTable::createTable()
    {
        return false;
    }

    bool ChatMsgCacheTable::queryCachedChatMsg(std::vector<MyChatMessageInfo> &vecFriednInfo, std::string &id)
    {
        MYSQL_RES* result=nullptr;
        std::string query="select * from chat_message_cache where toid=\""+id+"\"";
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),result))
        {
            _LOG(Logcxx::Level::ERRORS,"select * from chat_message_cache failed,query is:%s",query.c_str());
            return false;
        }

        //获取结果中的行数
        int rowCount=mysql_num_rows(result);
        //获取结果中的列数
        int colCount=mysql_num_fields(result);
        //存储列
        MYSQL_FIELD* pField=nullptr;
        //获取行
        MYSQL_ROW rowPtr=nullptr;
        //有就一直获取
        while(rowPtr=mysql_fetch_row(result))
        {
            //可以通过循环获取每一行的内容，每一行中
            //这里一行两列
            //类对象存储好友信息
            MyChatMessageInfo tmp;
            tmp.m_strFromId=rowPtr[0];
            tmp.m_strToId=rowPtr[1];
            tmp.m_strChatMsg=rowPtr[2];
            tmp.m_strSendName=rowPtr[3];
            vecFriednInfo.emplace_back(tmp);
        }
        mysql_free_result(result);
        return true;
    }

    bool ChatMsgCacheTable::insertCachedChatMsg(std::string &fromId, std::string &toId, std::string &msg, std::string &sendName, std::string &time)
    {
        std::string query="insert into chat_message_cache values(\""+fromId+"\",\""+toId+"\",\""+msg+"\",\""+sendName+"\",\""+time+"\")";
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),nullptr))
        {
            _LOG(Logcxx::Level::ERRORS,"insert into chat_message_cache failed,query is:%s",query.c_str());
            return false;
        }
        return true;
    }
    bool ChatMsgCacheTable::deleteCachedChatMsg(std::string &id)
    {
        std::string query="delete from chat_message_cache where toId=\""+id+"\"";
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),nullptr))
        {
            _LOG(Logcxx::Level::ERRORS,"delete from chat_message_cache failed,query is:%s",query.c_str());
            return false;
        }
        return true;
    }
}