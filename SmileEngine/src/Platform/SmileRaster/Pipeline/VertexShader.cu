#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdint.h>

__global__ void VertexShaderKernel()
{
	uint32_t index = (blockIdx.x * blockDim.x) + threadIdx.x;
}