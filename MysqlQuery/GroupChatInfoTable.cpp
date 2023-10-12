#include "GroupChatInfoTable.h"
#include "module/Log.h"

namespace database
{
    bool GroupChatInfoTable::createTable()
    {
        return false;
    }

    bool GroupChatInfoTable::getCurGroupId(std::string &groupId)
    {
        MYSQL_RES* result=nullptr;
        std::string query="select count(*) from group_properities";
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),result))
        {
            _LOG(Logcxx::Level::ERRORS,"select max(id) from group_properities failed,query is:%s",query.c_str());
            return false;
        }
        //获取count(*)的结果
        MYSQL_ROW rowPtr=nullptr;
        //有就一直获取
        while(rowPtr=mysql_fetch_row(result))
        {
            if(rowPtr[0]!=nullptr)
            {
                groupId=rowPtr[0];
            }
        }
        return true;
    }

    bool GroupChatInfoTable::insertGroupInfo(const std::string &groupId, const std::string &groupName, const std::string &groupIconPath)
    {
        return false;
    }
}