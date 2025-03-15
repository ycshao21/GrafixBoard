set(TARGET_NAME "stdasync_vs_stdexec")
set(CXX_SOURCES "${TARGET_NAME}.cpp")
add_executable(${TARGET_NAME} ${CXX_SOURCES})
target_link_libraries(
    ${TARGET_NAME} PRIVATE 
    ${PROJECT_NAMESPACE}GrafixCore
    STDEXEC::stdexec
)

set(TARGET_NAME "random")
set(CXX_SOURCES "math/random.cpp")
add_executable(${TARGET_NAME} ${CXX_SOURCES})
target_link_libraries(
    ${TARGET_NAME} PRIVATE 
    ${PROJECT_NAMESPACE}GrafixCore
    STDEXEC::stdexec
)

set(TARGET_NAME "bytestream")
set(CXX_SOURCES "utils/bytestream.cpp")
add_executable(${TARGET_NAME} ${CXX_SOURCES})
target_link_libraries(
    ${TARGET_NAME} PRIVATE 
    ${PROJECT_NAMESPACE}GrafixCore
    STDEXEC::stdexec
)