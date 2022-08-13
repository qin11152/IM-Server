#include <string>
#include <unistd.h>

inline std::string getCurrentDir()
{
    char buf[128]{0};
    getcwd(buf,sizeof(buf));
    return buf;
}