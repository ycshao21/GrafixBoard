#include <chrono>
#include <thread>

#include <GLFW/glfw3.h>

#include "grafixcore/core/platform.hpp"

namespace grafix::platform
{

auto getTime() -> float
{
    static auto start_time = std::chrono::high_resolution_clock::now();

    auto current_time = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration<float>(current_time - start_time).count();
    return duration;
    //  return static_cast<float>(glfwGetTime());
}

void sleep(float seconds)
{
    std::this_thread::sleep_for(std::chrono::duration<float>(seconds));
}

// // TODO: Make it glfw independent
// std::vector<const char*> get_required_extensions()
// {
//     uint32_t num_glfw_extensions = 0;
//     const char** glfw_extensions =
//         glfwGetRequiredInstanceExtensions(&num_glfw_extensions);
//     return std::vector<const char*>(glfw_extensions,
//                                     glfw_extensions + num_glfw_extensions);
// }

}  // namespace grafix::platform