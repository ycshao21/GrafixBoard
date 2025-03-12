#include <cstdint>
#include <cstdio>

#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <driver_types.h>

#include "_template_project_name_/math/vec_add.hpp"
#include "_template_project_name_/utils/address.hpp"

namespace _template_project_name_::cuda
{

__global__ void vec_add(const float* const a, const float* const b,
                        float* const c, const int n)
{
    const std::uint32_t threadIndex = threadIdx.x;
    std::uint32_t smId;
    asm volatile("mov.u32 %0, %smid;" : "=r"(smId));
    std::uint32_t warpId;
    asm volatile("mov.u32 %0, %warpid;" : "=r"(warpId));
    std::uint32_t laneId;
    asm volatile("mov.u32 %0, %laneid;" : "=r"(laneId));

    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n) {
        ::printf("Thread %d: %f + %f = %f\n", i, a[i], b[i], a[i] + b[i]);
        c[i] = a[i] + b[i];
        auto offset = computeOffset<std::uint32_t>(1, 2, 3, 4, 5, 6);
        ::printf("Offset: %d\n", offset);
        ::printf("SM: %d | Warp: %d | Lane: %d | Thread %d - Here!\n", smId,
                 warpId, laneId, threadIndex);
    }
}

void launch_vec_add(const float* const a, const float* const b, float* const c,
                    const int n)
{
    ::printf("Hello World from CUDA!\n");
    ::printf("Vector size: %d\n", n);
    std::uint32_t block_size = 256;
    std::uint32_t grid_size = (n + block_size - 1) / block_size;

    // Apply Device Memory
    float *d_a, *d_b, *d_c;
    cudaMalloc((void**) &d_a, n * sizeof(float));
    cudaMalloc((void**) &d_b, n * sizeof(float));
    cudaMalloc((void**) &d_c, n * sizeof(float));

    // Copy Host Memory to Device Memory
    cudaMemcpy(d_a, a, n * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, n * sizeof(float), cudaMemcpyHostToDevice);

    // Launch Kernel
    vec_add<<<grid_size, block_size>>>(d_a, d_b, d_c, n);

    // Copy Device Memory to Host Memory
    cudaMemcpy(c, d_c, n * sizeof(float), cudaMemcpyDeviceToHost);

    // Free Device Memory
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}

}  // namespace _template_project_name_::cuda