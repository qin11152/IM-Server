#include "../JsonBaseData.h"

class RegisterJsonData:public JsonBaseData
{
    public:
        RegisterJsonData(const std::string& message="");
        virtual void parse(const std::string& message) override;
        virtual std::string generateJson() override;
        
        std::string m_strUserId{""};
        std::string m_strUserPassword{""};
    private:
};