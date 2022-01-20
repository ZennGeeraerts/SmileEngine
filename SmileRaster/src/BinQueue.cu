#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		namespace BinQueue
		{
			__device__ void Push(Bin& bin, uint32_t triangleIndex)
			{
				int writeID = atomicAdd(&bin.QueueSize, 1);
				if (writeID < SMR_BIN_QUEUE_SIZE)
				{
					bin.Queue[writeID] = triangleIndex;
				}
			}

			__device__ void Clear(Bin& bin)
			{
				atomicExch(&bin.QueueSize, 0);
			}
		}
	}
}