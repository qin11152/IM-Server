/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-02 15:00:07
 * @FilePath: /IM-Server/protocol/AddFriendRequestJsonData.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "JsonBaseData.h"

namespace protocol
{
    class AddFriendRequestJsonData:public JsonBaseData
    {
    public:
        AddFriendRequestJsonData(const std::string& message="");
        void parse(const std::string& message="")override;
        std::string generateJson()override;

        std::string m_strMyId{ "" };
        std::string m_strFriendId{ "" };
        std::string m_strVerifyMsg{ "" };
        std::string m_strName{ "" };
        MessageType m_strType{ MessageType::AddFriendRequest };
    };
}