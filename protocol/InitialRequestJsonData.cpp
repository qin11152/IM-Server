#include "InitialRequestJsonData.h"

InitialRequestJsonData::InitialRequestJsonData(const std::string& message)
{
    parse(message);
}

void InitialRequestJsonData::parse(const std::string& message)
{
    if(message.empty())
    {
        return;
    }
    ptree m_ptree;
    std::stringstream sstream(message);
    read_json(sstream,m_ptree);
    m_strId=m_ptree.get<std::string>("UserId");
    return;
}

std::string InitialRequestJsonData::generateJson()
{
    ptree m_ptree;
    m_ptree.put("type",static_cast<int>(m_strType));
    m_ptree.put("UserId",m_strId);
    std::stringstream sstream;
    write_json(sstream,m_ptree);
    return sstream.str();
}