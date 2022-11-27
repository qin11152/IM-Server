/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-08-11 22:21:05
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2022-11-20 16:39:19
 * @FilePath: /src/IM-Server/PublicFunction.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <string>
#include <unistd.h>
#include <chrono>
#include <sstream>
#include <iomanip>

inline std::string getCurrentDir()
{
    char buf[128]{0};
    getcwd(buf,sizeof(buf));
    return buf;
}

inline std::string getCurrentTime()
{
    auto timet = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm timeInfo;
    timeInfo=*localtime(&timet);
    std::stringstream ss;
    ss << std::put_time(&timeInfo, "%F-%T");
    return ss.str();
}