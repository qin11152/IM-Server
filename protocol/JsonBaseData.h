#pragma once

#include "Mydefine.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include <string>
#include <type_traits>
#include <map>

using namespace boost::property_tree;
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
    private:
        std::map<std::string,std::string> m_mapMessageInfo;
};