set(TARGET_NAME "GrafixBoard")

file(GLOB_RECURSE CXX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${TARGET_NAME} ${CXX_SOURCES} ${CUDA_SOURCES})

target_link_libraries(
    ${TARGET_NAME} PRIVATE 
    # @see "lib/cpu/CMakeLists.txt"
    ${PROJECT_NAMESPACE}GrafixCore

)