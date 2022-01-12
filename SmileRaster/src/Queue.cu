#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		__device__ void Push(Segment& segment, uint32_t triangleID)
		{
			int writeID = atomicAdd(&segment.QueueSize, 1);
			if (writeID < 1024)
			{
				segment.Queue[writeID] = triangleID;
			}
		}

		__device__ void Clear(Segment& segment)
		{
			atomicExch(&segment.QueueSize, 0);
		}
	}
}