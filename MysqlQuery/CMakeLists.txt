cmake_minimum_required(VERSION 3.6)

AUX_SOURCE_DIRECTORY(. DIR_TEST1_SRCS)
ADD_LIBRARY (MysqlQueryLib ${DIR_TEST1_SRCS})

target_link_libraries(MysqlQueryLib  libmysqlclient.so)