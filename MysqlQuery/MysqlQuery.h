#pragma once

#include "Mydefine.h"
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

class MysqlQuery
{
    public:
        static std::shared_ptr<MysqlQuery> Instance();
        ~MysqlQuery();
        int InsertNewUser(const std::string name,const std::string&password,
                            const std::string iconUrl,const std::string& signature="");
        bool VertifyPassword(int id,const std::string& password);
        int GetCurrentUserCount();
        //像数据库添加好友
        bool AddFriend(std::string friend_1,std::string friend_2);
        //查询用户是否在线
        bool queryUserIsOnline(std::string userId);
        //查询用户是否存在
        bool queryUserIsExist(std::string userId);
        //把添加好友的信息缓存起来
        bool insertAddFriendCache(const std::string& requestId,const std::string& destinationId,const std::string& verifyMsg);
        //把获取的好友列表信息存储在vector中
        void queryUserFrinedList(std::vector<FriendInfo>& vecFriendList,std::string& strUserId);


    private:
        MysqlQuery(const char* ip,const char* database,const char* user,const char*password);
        static std::shared_ptr<MysqlQuery> m_ptrInstance;
        static std::mutex m_mutex;
        MYSQL* m_mysql{nullptr};
        MYSQL_RES* m_mysqlQueryResult{nullptr};
        MYSQL_ROW m_mysqlRow;
        std::string m_destinationIp{""};
        std::string m_destinationDatabase{""};
        std::string m_destinationUser{""};
        std::string m_destinationPassword{""};

        //
        void initMysql();
};
