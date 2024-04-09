#include "AddFriendCacheTable.h"
#include "module/Log.h"

bool database::AddFriendCacheTable::createTable()
{
    return false;
}

bool database::AddFriendCacheTable::insertAddFriendCache(const std::string &requestId, const std::string &destinationId, const std::string &verifyMsg)
{
    std::string query="insert into add_friend values(\""+requestId+"\",\""+destinationId+"\",\""+verifyMsg+"\")";
    if(!DataBaseOperate::Instance()->execQuery(query.c_str(),nullptr))
    {
        _LOG(Logcxx::Level::ERRORS,"insert into add_friend failed,query is:%s",query.c_str());
        return false;
    }
    return true;
}

bool database::AddFriendCacheTable::queryCachedAddFriendInfo(std::vector<MyAddFriendInfo> &vecFriednInfo, std::string &id)
{
    MYSQL_RES* result=nullptr;
    std::string query="select * from add_friend where myId=\""+id+"\"";
    if(!DataBaseOperate::Instance()->execQuery(query.c_str(),&result))
    {
        _LOG(Logcxx::Level::ERRORS,"select * from add_friend failed,query is:%s",query.c_str());
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
        MyAddFriendInfo tmp;
        tmp.m_strFriendId=rowPtr[0];
        tmp.m_strMyId=rowPtr[1];
        tmp.m_strVerifyMsg=rowPtr[2];
        vecFriednInfo.push_back(tmp);
    }
    mysql_free_result(result);
    return true;
}

bool database::AddFriendCacheTable::deleteCachedAddFriendInfo(std::string &id)
{
    std::string query="delete from add_friend where myId=\""+id+"\"";
    if(!DataBaseOperate::Instance()->execQuery(query.c_str(),nullptr))
    {
        _LOG(Logcxx::Level::ERRORS,"delete from add_friend failed,query is:%s",query.c_str());
        return false;
    }
    return true;
}
