set(PROJECT_NAME grpc-server)
project(${PROJECT_NAME})

add_compile_definitions(APP_NAME="${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_DIR "source")
set(INCLUDE_DIR "include")

include_directories(${INCLUDE_DIR} ${SOURCE_DIR})

add_executable(${PROJECT_NAME}-sync ${SOURCE_FILES} "${SOURCE_DIR}/main_sync.cpp")
add_executable(${PROJECT_NAME}-async ${SOURCE_FILES} "${SOURCE_DIR}/main_async.cpp")
add_executable(${PROJECT_NAME}-cb ${SOURCE_FILES} "${SOURCE_DIR}/main_cb.cpp")

foreach(target ${PROJECT_NAME}-sync ${PROJECT_NAME}-async ${PROJECT_NAME}-cb)
    target_link_libraries(${target}
        PRIVATE
        proto
    )
    target_include_directories(${target}
        PRIVATE
        ${proto_INCLUDE_DIRS}
    )
endforeach()

add_subdirectory(tests)