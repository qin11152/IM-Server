#pragma once
#include "DataBaseOperate.h"

namespace database
{
    class UserInfoTable
    {
        public:
            UserInfoTable()=default;
            ~UserInfoTable()=default;
            UserInfoTable(const UserInfoTable&)=delete;
            UserInfoTable& operator=(const UserInfoTable&)=delete;

            /**
             * @brief Create a Table object
             * 
             * @return true 
             * @return false 
             */
            bool createTable();

            /**
             * @brief 新注册的用户插入到表中
             * 
             * @param name 名字
             * @param password 密码
             * @param iconUrl 头像路径
             * @param signature 签名
             * @param returnId 返回的id
             * @return true 
             * @return false 
             */
            bool insertNewUser(int& returnId,const std::string name,const std::string&password,
                            const std::string iconUrl,const std::string& signature="");

            /**
             * @description: 验证用户名和密码是否正确
             * @param {std::string} name
             * @param {std::string} password
             * @return {*}
             */
            bool vertifyPassword(int id,const std::string& password);

            /**
             * @brief Get the Current User Count object
             * 
             * @param count 返回已有数量
             * @return true 
             * @return false 
             */
            bool getCurrentUserCount(int& count);

            /**
             * @brief 查询用户是否在线
             * 
             * @param userId 用户的id
             * @return true 
             * @return false 
             */
            bool queryUserIsOnline(std::string userId);
            
            /**
             * @brief 修改用户在线状态
             * 
             * @param userId 用户id
             * @param onlineState 要修改的状态
             * @return true 
             * @return false 
             */
            bool updateUserOnlineState(std::string userId,bool onlineState);

            /**
             * @brief 查询用户是否存在
             * 
             * @param userId 用户id
             * @return true 
             * @return false 
             */
            bool queryUserIsExist(std::string userId);

            /**
             * @brief 根据id查找对应的昵称
             * 
             * @param id 用户id
             * @return std::string 昵称
             */
            std::string queryUserNameAcordId(const std::string &id);
            /**
             * @brief 将该用户的头像路径插入表中
             * 
             * @param id 用户id
             * @param strIamgePath 头像的路径 
             * @param timeStamp 头像时间戳
             * @return true 
             * @return false 
             */
            bool updateImagePathAcordId(const std::string& id,const std::string strIamgePath,const std::string& timeStamp);

            /**
             * @brief 根据id获取头像的路径
             * 
             * @param id 用户id
             * @param strImagePath std::string 此用户的头像路径 
             * @return true 
             * @return false 
             */
            bool queryImagePathAcordId(const std::string& id,std::string& strImagePath);
            

            /**
             * @brief 根据id获取头像时间戳
             * 
             * @param id 用户id
             * @param strImageTimeStamp 返回的头像时间戳
             * @return true 
             * @return false 
             */
            bool queryImageTimeStampAcordId(const std::string& id,std::string& strImageTimeStamp);
        private:
    };
}