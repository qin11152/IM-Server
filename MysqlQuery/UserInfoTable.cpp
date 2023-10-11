#include "UserInfoTable.h"
#include "module/Log.h"

namespace database
{
    bool UserInfoTable::createTable()
    {
        return false;
    }
    bool UserInfoTable::insertNewUser(int& returnId,const std::string name, const std::string &password, const std::string iconUrl, const std::string &signature)
    {
        std::string profileImagePath="./data/profileImage/DefaultImage.jpg";
        //获取当前时间转换为字符串
        std::string time=getCurrentTime();
        getCurrentUserCount(returnId);
        returnId++;
        std::string query="insert into user_info (id,password,name,signature,icon,online,image,imagetimestamp) values("
        +std::to_string(returnId)+",\""+password+"\",\""+name+"\",\""+signature+
        "\",\""+iconUrl+"\",false,\""+profileImagePath+"\",\""+time+"\");";
        MYSQL_RES* result=nullptr;
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),result))
        {
            _LOG(Logcxx::ERROR,"insert into user_info failed,query is:%s",query.c_str());
            return false;
        }
        mysql_free_result(result);
        return true;
    }

    bool UserInfoTable::vertifyPassword(int id, const std::string &password)
    {
        MYSQL_RES* result=nullptr;
        std::string query="select * from user_info";
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),result))
        {
            _LOG(Logcxx::ERROR,"select * from user_info failed");
            return false;
        }
        auto mysqlRow=mysql_fetch_row(result);
        if(mysqlRow[1]==password)
        {
            mysql_free_result(result);
            return true;
        }
        else
        {
            mysql_free_result(result);
            return false;
        }
    }
    bool UserInfoTable::getCurrentUserCount(int &count)
    {
        MYSQL_RES* result=nullptr;
        std::string query="select count(*) from user_info";
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),result))
        {
            _LOG(Logcxx::ERROR,"select count(*) from user_info failed");
            return false;
        }
        auto mysqlRow=mysql_fetch_row(result);
        count=atoi(mysqlRow[0]);
        mysql_free_result(result);
        return true;
    }
    bool UserInfoTable::queryUserIsOnline(std::string userId)
    {
        MYSQL_RES* result=nullptr;
        bool onlineState={false};
        std::string query="select online from user_info where id="+userId;
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),result))
        {
            _LOG(Logcxx::ERROR,"insert into friend_info failed,query is:%s",query.c_str());
            return false;
        }
        //获取结果中的行数
        int rowCount=mysql_num_rows(result);
        //获取结果中的列数
        int colCount=mysql_num_fields(result);

        //当还有列的时候就一直获取
        //获取行
        MYSQL_ROW rowPtr=nullptr;
        //有就一直获取
        while(rowPtr=mysql_fetch_row(result))
        {
            //可以通过循环获取每一行的内容，每一行中
            //列的名称在上边已经进行了获取
            //这里只有一行一列
            int tmpi=atoi(rowPtr[0]);
            onlineState=tmpi==0?false:true;
        }
        mysql_free_result(result);
        return onlineState;
    }
    bool UserInfoTable::updateUserOnlineState(std::string userId, bool onlineState)
    {
        MYSQL_RES* result=nullptr;
        std::string strOnlineState=onlineState?"1":"0";
        std::string query="update user_info set online="+strOnlineState+" where id="+userId;
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),result))
        {
            _LOG(Logcxx::ERROR,"update user_info set online failed,query is:%s",query.c_str());
            return false;
        }
        return true;
    }
    bool UserInfoTable::queryUserIsExist(std::string userId)
    {
        MYSQL_RES* result=nullptr;
        bool isExist=false;
        std::string query="select count(*) from user_info where id="+userId;
        if(DataBaseOperate::Instance()->execQuery(query.c_str(),result))
        {
            _LOG(Logcxx::ERROR,"select count(*) from user_info failed,query is:%s",query.c_str());
            return false;
        }
        //获取结果中的行数
        int rowCount=mysql_num_rows(result);

        //获取行
        MYSQL_ROW rowPtr=nullptr;
        //有就一直获取
        while(rowPtr=mysql_fetch_row(result))
        {
            //可以通过循环获取每一行的内容，每一行中
            //列的名称在上边已经进行了获取
            //这里只有一行一列
            isExist=rowPtr[0]==0?false:true;
        }
        mysql_free_result(result);
        return isExist;
    }
}