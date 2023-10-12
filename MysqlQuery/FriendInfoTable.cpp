#include "FriendInfoTable.h"
#include "module/Log.h"

namespace database
{
    bool FriendInfoTable::insertFriendInfo(std::string friendId_1, std::string friendId_2, std::string friend1name, std::string friend2name)
    {
        MYSQL_RES* result=nullptr;
        std::string query="insert into friend_info values(\""+ friendId_1 +"\",\""+ friendId_2 +"\",\""+friend2name+"\")";
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),result))
        {
            _LOG(Logcxx::Level::ERRORS,"insert into friend_info failed,query is:%s",query.c_str());
            return false;
        }
        query="insert into friend_info values(\""+ friendId_2 +"\",\""+ friendId_1 +"\",\""+friend1name+"\")";
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),result))
        {
            _LOG(Logcxx::Level::ERRORS,"insert into friend_info failed,query is:%s",query.c_str());
            return false;
        } 
        return true;
    }
    bool FriendInfoTable::queryUserFrinedList(std::vector<FriendInfo> &vecFriendList, std::string &strUserId)
    {
        MYSQL_RES* result=nullptr;
        std::string query="SELECT a.id_friend, a.name, b.imagetimestamp from friend_info as a inner JOIN user_info as b on (a.id_friend=b.id and a.id_my=\""+strUserId+"\") order by CONVERT( a.`name` USING gbk) COLLATE gbk_chinese_ci DESC;";
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),result))
        {
            _LOG(Logcxx::Level::ERRORS,"select id_friend, name from friend_info failed,query is:%s",query.c_str());
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
            FriendInfo tmp;
            tmp.m_strFriendId=rowPtr[0];
            tmp.m_strFriendName=rowPtr[1];
            tmp.m_strImageTimeStamp=rowPtr[2];
            vecFriendList.push_back(tmp);
        }
        mysql_free_result(result);
        return true;
    }
}