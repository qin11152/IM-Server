#include "ProfileImageMsgJsonData.h"

ProfileImageMsgJsonData::ProfileImageMsgJsonData(const std::string& message/*=""*/)
{
    parse(message);
}

void ProfileImageMsgJsonData::parse(const std::string& message)
{
    if (message.empty())
    {
        return;
    }
    ptree m_ptree;
    std::stringstream sstream(message);
    read_json(sstream, m_ptree);
    m_strId = m_ptree.get<std::string>("UserId");
    m_strUUID = m_ptree.get<std::string>("ImageUUID");
    m_strBase64Msg = m_ptree.get<std::string>("ImageBase64Msg");
    m_strImageName = m_ptree.get<std::string>("ImageName");
    m_iCurIndex = m_ptree.get<int>("ImageIndex");
    m_iSumIndex = m_ptree.get<int>("SumIndex");
}

std::string ProfileImageMsgJsonData::generateJson()
{
    ptree m_ptree;

    m_ptree.put("type",static_cast<int>(m_strType));
    m_ptree.put("UserId",m_strId);
    m_ptree.put("ImageUUID",m_strUUID);
    m_ptree.put("ImageBase64Msg",m_strBase64Msg);
    m_ptree.put("ImageName",m_strImageName);
    m_ptree.put("ImageIndex",m_iCurIndex);
    m_ptree.put("SumIndex",m_iSumIndex);

    std::stringstream sstream;
    write_json(sstream,m_ptree);
    return sstream.str();
}
