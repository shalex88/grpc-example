set(PROJECT_NAME grpc-example)
include(cmake/SetVersion.cmake)
include(cmake/EnableVcpkg.cmake)
cmake_minimum_required(VERSION 3.22)
project(${PROJECT_NAME} LANGUAGES CXX VERSION ${VERSION})

set(CMAKE_CXX_STANDARD 20)

find_package(gRPC CONFIG REQUIRED)

function(link_common_libraries target)
    target_link_libraries(${target}
        gRPC::grpc++
    )
endfunction(link_common_libraries)

add_subdirectory(client)
add_subdirectory(server)