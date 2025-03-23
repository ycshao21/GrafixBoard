set(TARGET_NAME "random")
set(CXX_SOURCES "math/random.cpp")
add_executable(${TARGET_NAME} ${CXX_SOURCES})
target_link_libraries(
    ${TARGET_NAME} PRIVATE 
    ${PROJECT_NAMESPACE}GrafixCore
)

set(TARGET_NAME "bytestream")
set(CXX_SOURCES "utils/bytestream.cpp")
add_executable(${TARGET_NAME} ${CXX_SOURCES})
target_link_libraries(
    ${TARGET_NAME} PRIVATE 
    ${PROJECT_NAMESPACE}GrafixCore
)