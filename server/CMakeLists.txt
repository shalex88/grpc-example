set(PROJECT_NAME grpc-server-example)
project(${PROJECT_NAME})

add_compile_definitions(APP_NAME="${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_DIR "source")
set(INCLUDE_DIR "include")

include_directories(${INCLUDE_DIR} ${SOURCE_DIR})
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PROTO_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    proto
)

add_subdirectory(tests)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)