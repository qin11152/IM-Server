#include "HeartPackageJsonData.h"

HeartPackageJsonData::HeartPackageJsonData(const std::string& message)
{
    parse(message);
}
void HeartPackageJsonData::parse(const std::string& message)
{
if (message.empty())
    {
        return;
    }
    ptree m_ptree;
    std::stringstream sstream(message);
    read_json(sstream, m_ptree);
    return;
}