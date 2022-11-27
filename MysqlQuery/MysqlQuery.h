/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2022-11-27 13:15:27
 * @FilePath: /src/IM-Server/MysqlQuery/MysqlQuery.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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
        //将新注册的用户插入数据库中
        int InsertNewUser(const std::string name,const std::string&password,
                            const std::string iconUrl,const std::string& signature="");
        bool VertifyPassword(int id,const std::string& password);
        int GetCurrentUserCount();
        //像数据库添加好友
        /*
        *param friend_1:插入到数据库的id_my
        *param friend_2：插入到数据库的id_friend
        *param name：friend对应的的name
        */
        bool AddFriend(std::string friend_1,std::string friend_2,std::string friend1name,std::string friend2name);
        //查询用户是否在线
        bool queryUserIsOnline(std::string userId);
        //修改用户在线状态
        bool updateUserOnlineState(std::string userId,bool onlineState);
        //查询用户是否存在
        bool queryUserIsExist(std::string userId);
        //把添加好友的信息缓存起来
        bool insertAddFriendCache(const std::string& requestId,const std::string& destinationId,const std::string& verifyMsg);
        //从数据库中得到缓存的添加好友的消息
        bool queryCachedAddFriendInfo(std::vector<MyAddFriendInfo>& vecFriednInfo,std::string& id);
        //获取完就删除了缓存的添加好友消息
        bool deleteCachedAddFriendInfo(std::string& id);
        //从数据库中得到缓存的聊天消息
        bool queryCachedChatMsg(std::vector<MyChatMessageInfo>& vecFriednInfo,std::string& id);
        //把点对点聊天消息缓存起来，好友不在线时用的
        bool insertCachedChatMsg(std::string& fromId,std::string& toId,std::string& msg,std::string& sendName,std::string& time);
        //从数据库中删除缓存的聊天消息
        bool deleteCachedChatMsg(std::string& id);
        //把获取的好友列表信息存储在vector中
        void queryUserFrinedList(std::vector<FriendInfo>& vecFriendList,std::string& strUserId);
        //根据id查找名称
        std::string queryUserNameAcordId(const std::string &id);
        //根据id插入图片到数据库中
        bool updateImagePathAcordId(const std::string& id,const std::string strIamgePath,const std::string& timeStamp);
        //获取头像图片的路径
        std::string queryImagePathAcordId(const std::string& id);

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
