set(PROJECT_NAME grpc-client-example)
project(${PROJECT_NAME})

add_compile_definitions(APP_NAME="${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_DIR "source")
set(INCLUDE_DIR "include")

# Set variables for protobuf usage
set(PROTO_DIR "proto")
set(GENERATED_DIR "generated")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_DIR})
# find_package(absl CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
file(GLOB_RECURSE PROTO_FILES CONFIGURE_DEPENDS "${PROTO_DIR}/*.proto")

include_directories(${INCLUDE_DIR} ${SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_DIR} ${Protobuf_INCLUDE_DIRS})
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PROTO_SOURCE_FILES})

link_common_libraries(${PROJECT_NAME})

protobuf_generate(
        TARGET ${PROJECT_NAME}
        PROTOS ${PROTO_FILES}
        OUT_VAR PROTO_SOURCE_FILES
        PROTOC_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_DIR}
)

add_subdirectory(tests)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)