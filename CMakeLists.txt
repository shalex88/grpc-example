set(PROJECT_NAME grpc-example)
include(cmake/SetVersion.cmake)
include(cmake/EnableVcpkg.cmake)
cmake_minimum_required(VERSION 3.22)
project(${PROJECT_NAME} LANGUAGES CXX VERSION ${VERSION})

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(proto)
add_subdirectory(client)
add_subdirectory(server)