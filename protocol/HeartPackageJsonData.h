#include "JsonBaseData.h"

class HeartPackageJsonData :
    public JsonBaseData
{
public:
    HeartPackageJsonData(const std::string& message="");
    void parse(const std::string& message)override;

    MessageType m_strType{ MessageType::HeartPackage};
};