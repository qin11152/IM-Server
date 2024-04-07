cmake_minimum_required(VERSION 3.15)

project(httpserver)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

aux_source_directory(. srcs)

add_executable(httpserver ${srcs})

target_link_libraries(httpserver pthread PocoFoundation PocoNet PocoUtil)