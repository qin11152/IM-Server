/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2023-10-12 20:08:26
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2024-04-09 11:08:15
 * @FilePath: /IM-Server/MysqlQuery/GroupChatInfoTable.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "GroupChatInfoTable.h"
#include "module/Log.h"

namespace database
{
    bool GroupChatInfoTable::createTable()
    {
        std::string query="CREATE TABLE group_properities11 (groupId int NOT NULL,selfDefineName varchar(255),iconPath varchar(255),PRIMARY KEY (groupId))";
        if(!DataBaseOperate::Instance()->execQuery(query.c_str(),nullptr))
        {
            _LOG(Logcxx::Level::ERRORS,"create table group_properities failed,query is:%s",query.c_str());
            return false;
        }
        return true;
    }

    bool GroupChatInfoTable::getCurGroupId(std::string &groupId)
    {
        MYSQL_RES* result=nullptr;
        std::string query="select count(*) from group_properities";
        if(!DataBaseOperate::Instance()->execQuery(query.c_str(),&result))
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
        std::string query="insert into group_properities values(/"+groupId+","+groupName+","+groupIconPath+")";
        if(!DataBaseOperate::Instance()->execQuery(query.c_str(),nullptr))
        {
            _LOG(Logcxx::Level::ERRORS,"insert into group_properities failed,query is:%s",query.c_str());
            return false;
        }
        return true;
    }
}