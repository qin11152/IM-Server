/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-08-11 22:21:05
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2022-08-14 18:51:20
 * @FilePath: /src/IM-Server/PublicFunction.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <string>
#include <unistd.h>

inline std::string getCurrentDir()
{
    char buf[128]{0};
    getcwd(buf,sizeof(buf));
    return buf;
}

inline std::string getCurrentTime()
{
    time_t t = time(0);
    char tmp[64]{0};
    strftime(tmp,sizeof(tmp),"%Y-%m-%d %H:%M:%S",localtime(&t));
    return tmp;
}