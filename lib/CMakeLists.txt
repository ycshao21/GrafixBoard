set(LIB_NAME "GrafixCore")

file(GLOB_RECURSE CXX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${LIB_NAME} ${CXX_SOURCES})
add_library(${PROJECT_NAMESPACE}${LIB_NAME} ALIAS ${LIB_NAME})

target_include_directories(
    ${LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(${LIB_NAME} 
    PRIVATE $<$<BOOL:${BUILD_SHARED_LIBS}>:GRAFIXCORE_EXPORT>
    INTERFACE $<$<BOOL:${BUILD_SHARED_LIBS}>:GRAFIXCORE_IMPORT>
)

target_link_libraries(
    ${LIB_NAME} 
    PUBLIC
    ${Vulkan_LIBRARIES}
    glfw
    msft_proxy
    spdlog::spdlog
    cxxopts::cxxopts
    EnTT::EnTT
    imgui::imgui
)

# @see "cmake/utils/lib-install.cmake"
install_libraries(${LIB_NAME} ${PROJECT_NAMESPACE} ${LIB_NAME})