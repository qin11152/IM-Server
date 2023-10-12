cmake_minimum_required(VERSION 3.6)

project(qin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

#cmake中指定生成64位程序
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_BUILD_TYPE "Release")

aux_source_directory(. DIR_SRCS)
aux_source_directory(./protocol DIR_SRCS)
aux_source_directory(./protocol/ImageJsonData DIR_SRCS)
aux_source_directory(./module DIR_SRCS)
aux_source_directory(./MysqlQuery DIR_SRCS)
aux_source_directory(./ChatClient DIR_SRCS)
aux_source_directory(./ChatServer DIR_SRCS)
include_directories("${PROJECT_SOURCE_DIR}/MysqlQuery")
include_directories("${PROJECT_SOURCE_DIR}/ChatServer")
include_directories("${PROJECT_SOURCE_DIR}/ChatClient")
include_directories("${PROJECT_SOURCE_DIR}/protocol")
include_directories("${PROJECT_SOURCE_DIR}/protocol/ImageJsonData")
include_directories("${PROJECT_SOURCE_DIR}/module")
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")

if(WIN32)
    include_directories("C:/Program Files/MySQL/MySQL Server 8.0/include")
endif()

# add_subdirectory(MysqlQuery)
# add_subdirectory(ChatClient)
# add_subdirectory(ChatServer)
# add_subdirectory(protocol)
# add_subdirectory(module)

link_directories("${PROJECT_SOURCE_DIR}/lib")

add_executable(qin ${DIR_SRCS})

# TARGET_LINK_LIBRARIES(qin MysqlQueryLib)
# TARGET_LINK_LIBRARIES(qin ChatServerLib)
# TARGET_LINK_LIBRARIES(qin ChatClientLib)
# TARGET_LINK_LIBRARIES(qin ProtocolLib)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(qin mysqlclient)
elif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(qin mysqlclient boost_thread log4cxx)
else()
    target_link_libraries(qin mysqlclient log4cxx)
endif()
# target_link_libraries(qin LogLib)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
endif()