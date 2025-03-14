set(TARGET_NAME "stdasync_vs_stdexec")

set(CXX_SOURCES "stdasync_vs_stdexec.cpp")

add_executable(${TARGET_NAME} ${CXX_SOURCES})

target_link_libraries(
    ${TARGET_NAME} PRIVATE 
    ${PROJECT_NAMESPACE}GrafixCore
    STDEXEC::stdexec
)