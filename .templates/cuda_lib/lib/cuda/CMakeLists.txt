set(LIB_NAME "templateCudaLib")

file(GLOB_RECURSE CXX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE CUDA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cu)

add_library(${LIB_NAME} ${CXX_SOURCES} ${CUDA_SOURCES})
add_library(${PROJECT_NAMESPACE}${LIB_NAME} ALIAS ${LIB_NAME})

target_include_directories(
    ${LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    ${LIB_NAME} 
    PUBLIC 
    CUDA::cudart 
    CUDA::cublas
)

target_compile_definitions(${LIB_NAME} 
    PRIVATE $<$<BOOL:${BUILD_SHARED_LIBS}>:_template_project_name__EXPORT>
    INTERFACE $<$<BOOL:${BUILD_SHARED_LIBS}>:_template_project_name__IMPORT>
)

# @see "cmake/utils/lib-install.cmake"
install_libraries(${LIB_NAME} ${PROJECT_NAMESPACE} ${LIB_NAME})