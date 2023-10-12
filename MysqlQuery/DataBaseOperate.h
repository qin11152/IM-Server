#pragma once

#include "Mydefine.h"
#if defined(__linux__)
    #include <mysql/mysql.h>
    #include <mysql/mysqld_error.h>
#elif defined(_WIN32)
    #include <mysql.h>
    #include <mysqld_error.h>
#endif
#include <stdio.h>
#include <string>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

namespace database
{
    class DataBaseOperate
    {
        public:
            static std::shared_ptr<DataBaseOperate> Instance();
            ~DataBaseOperate();

            /**
             * @brief 执行数据库查询操作
             * 
             * @param query 查询的语句
             * @param result 查询的结果
             */
            bool execQuery(const char* query,MYSQL_RES* result);
            
        private:
            DataBaseOperate(const char* ip,const char* database,const char* user,const char*password);
            static std::shared_ptr<DataBaseOperate> m_ptrInstance;
            static std::mutex m_mutex;
            std::mutex m_mutexForMysql;
            MYSQL* m_mysql{nullptr};
            std::string m_destinationIp{""};
            std::string m_destinationDatabase{""};
            std::string m_destinationUser{""};
            std::string m_destinationPassword{""};

            //
            void initMysql();
    };
}