#include "JsonBaseData.h"

class RegisterReplyData:public JsonBaseData
{
private:
    /* data */
public:
    RegisterReplyData(const std::string& message="");
    virtual ~RegisterReplyData()override{}
    virtual void parse(const std::string& message)override;
    virtual std::string generateJson()override;

    bool m_bRegisterResult{false};
    int m_iId{-1};
};
