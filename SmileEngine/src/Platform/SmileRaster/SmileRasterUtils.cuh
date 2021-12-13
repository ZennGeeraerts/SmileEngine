#include "curand_kernel.h"
#include <stdio.h>

namespace Smile
{
    #define GPU_ERROR_CHECK(ans) { GPUAssert((ans), __FILE__, __LINE__); }
    inline void GPUAssert(cudaError_t error, const char* pFile, int line, bool bAbort = true)
    {
        if (error != cudaSuccess)
        {
            fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(error), pFile, line);
            if (bAbort)
                exit(error);
        }
    }
}