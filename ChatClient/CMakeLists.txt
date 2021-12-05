cmake_minimum_required(VERSION 3.7)

include_directories("${PROJECT_SOURCE_DIR}/protocol")
include_directories("${PROJECT_SOURCE_DIR}/MysqlQuery")

AUX_SOURCE_DIRECTORY(. DIR_TEST1_SRCS)
ADD_LIBRARY (ChatClientLib ${DIR_TEST1_SRCS})

TARGET_LINK_LIBRARIES(ChatClientLib MysqlQueryLib)
TARGET_LINK_LIBRARIES(ChatClientLib ProtocolLib)