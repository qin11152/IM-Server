cmake_minimum_required(VERSION 3.6)

project(qin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

aux_source_directory(. DIR_SRCS)
aux_source_directory(./protocol DIR_SRCS)
aux_source_directory(./module DIR_SRCS)
aux_source_directory(./MysqlQuery DIR_SRCS)
aux_source_directory(./ChatClient DIR_SRCS)
aux_source_directory(./ChatServer DIR_SRCS)
include_directories("${PROJECT_SOURCE_DIR}/MysqlQuery")
include_directories("${PROJECT_SOURCE_DIR}/ChatServer")
include_directories("${PROJECT_SOURCE_DIR}/ChatClient")
include_directories("${PROJECT_SOURCE_DIR}/protocol")
include_directories("${PROJECT_SOURCE_DIR}/module")



# add_subdirectory(MysqlQuery)
# add_subdirectory(ChatClient)
# add_subdirectory(ChatServer)
# add_subdirectory(protocol)
# add_subdirectory(module)

add_executable(qin ${DIR_SRCS})

# TARGET_LINK_LIBRARIES(qin MysqlQueryLib)
# TARGET_LINK_LIBRARIES(qin ChatServerLib)
# TARGET_LINK_LIBRARIES(qin ChatClientLib)
# TARGET_LINK_LIBRARIES(qin ProtocolLib)
target_link_libraries(qin libmysqlclient.so boost_thread log4cxx)
# target_link_libraries(qin LogLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")