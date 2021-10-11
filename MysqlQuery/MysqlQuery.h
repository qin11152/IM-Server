#pragma once

#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
#include <stdio.h>
#include <string>
#include <iostream>

class MysqlQuery{
    public:
        MysqlQuery(const char* ip,const char* database,const char* user,const char*password);
        ~MysqlQuery();
        bool InsertNewUser(const std::string name,const std::string&password,
                            const std::string iconUrl,const std::string& signature="");
        bool VertifyPassword(int id,const std::string& password);
        int GetCurrentUserCount();
        bool AddFriend(int friend_1,int friend_2);
    private:
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