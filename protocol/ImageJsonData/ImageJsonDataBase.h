/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2023-03-24 20:26:54
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-03-24 20:34:00
 * @FilePath: /IM-Server/protocol/ImageJsonData/ImageJsonDataBase.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "protocol/JsonBaseData.h"

using namespace boost::property_tree;
//all the json data used in communicate should inherit from this class


class ImageInnerJsonData
{
public:
    ImageInnerJsonData(){}
    virtual ~ImageInnerJsonData() {};
    virtual void parse(const std::string& message) {};
    virtual std::string generateJson() { return ""; }
private:
};

class ImageJsonBaseData
{
public:
    ImageJsonBaseData()
    {
    }
    virtual ~ImageJsonBaseData()
    {
    }
    ImageJsonBaseData(const ImageJsonBaseData& l) = delete;
    ImageJsonBaseData& operator=(const ImageJsonBaseData& l) = delete;
    virtual void parse(const std::string& message) = 0;
    virtual std::string generateJson() = 0;

    int m_iImageLenth{ 0 };     //图片的长度
    std::string m_strImageSuffix{ "" };
    ImageInnerJsonData m_stInnerData;   //内部可包含一层数据，可为空

    MessageType m_strType{ MessageType::InfoWithImage };

private:
};

