/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2023-10-12 20:08:26
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2024-04-09 11:07:49
 * @FilePath: /IM-Server/MysqlQuery/DataBaseOperate.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "protocol/Mydefine.h"
#include "module/Log.h"

#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
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
            bool execQuery(const char* query,MYSQL_RES** result);
            
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