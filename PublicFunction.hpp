/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-08-11 22:21:05
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-10-12 20:41:59
 * @FilePath: /src/IM-Server/PublicFunction.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <string>
#if defined(__linux__)
    #include <unistd.h>
#elif defined(_WIN32)
#endif
#include <chrono>
#include <sstream>
#include <iomanip>

inline std::string getCurrentDir()
{
    char buf[128]{0};
    //windows下没有getcwd函数，用GetCurrentDirectory代替
    #if defined(__linux__)
        auto returnValue=getcwd(buf,sizeof(buf));
    #elif defined(_WIN32)
        GetCurrentDirectory(sizeof(buf),buf);
    #endif
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