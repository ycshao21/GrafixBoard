set(TARGET_NAME "main")

file(GLOB_RECURSE CXX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE CUDA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cu)

add_executable(${TARGET_NAME} ${CXX_SOURCES} ${CUDA_SOURCES})

target_include_directories(
    ${TARGET_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
    ${TARGET_NAME} PRIVATE 
    CUDA::cudart 
    CUDA::cublas
)