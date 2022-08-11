#include <string>
#include <unistd.h>

std::string getCurrentDir()
{
    char buf[128]{0};
    getcwd(buf,sizeof(buf));
    return buf;
}