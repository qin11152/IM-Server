/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-02 14:59:45
 * @FilePath: /IM-Server/protocol/JsonBaseData.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "Mydefine.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

 
#include <string>
#include <type_traits>

using namespace boost::property_tree;
using namespace rapidjson;

namespace protocol
{
    //all the json data used in communicate should inherit from this class
    class JsonBaseData
    {
        public:
            JsonBaseData(){}
            virtual ~JsonBaseData(){}
            virtual MessageType getType()const{return m_strType;}
            virtual void parse(const std::string& message)=0;
            virtual std::string generateJson()=0;
            MessageType m_strType{MessageType::Default};
    };
}