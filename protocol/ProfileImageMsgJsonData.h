/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-08-13 20:08:33
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-02 15:10:23
 * @FilePath: /src/IM-Server/protocol/ProfileImageMsgJsonData.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "JsonBaseData.h"

namespace protocol
{
    class ProfileImageMsgJsonData :public JsonBaseData
    {
    public:
        ProfileImageMsgJsonData(const std::string& message = "");
        void parse(const std::string& message)override;
        std::string generateJson()override;

        std::string m_strId{ "" };
        std::string m_strUUID{ "" };        //唯一标识，表明是这个图片
        std::string m_strBase64Msg{ "" };   //基于BASE64编码的图片信息
        std::string m_strImageName{ "" };   //图片的名称
        std::string m_strSuffix{ "" };      //图片后缀类型
        std::string m_strTimeStamp{ "" };   //时间戳,发送到服务端就不一样了，所以一并发送过去
        int m_iCurIndex{ 0 };              //当前处于分的第几段
        int m_iSumIndex{ 0 };               //总共分了几段
        ProfileImageType m_eImageType{ ProfileImageType::UpdateProfileImage };  //传递头像的需求

        MessageType m_strType{ MessageType::ProfileImageMsg };
    };
} // namespace protocol
