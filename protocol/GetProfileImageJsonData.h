/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-11-27 16:45:56
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2022-11-27 16:49:59
 * @FilePath: /src/IM-Server/protocol/GetProfileImageJsonData.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once 

#include "JsonBaseData.h"

class GetProfileImageJsonData:public JsonBaseData
{
public:
    GetProfileImageJsonData(const std::string& message="");
    void parse(const std::string& message);
    std::string generateJson();

    std::string m_strId;
    MessageType m_strType{ MessageType::getFriendProfileImage };
};