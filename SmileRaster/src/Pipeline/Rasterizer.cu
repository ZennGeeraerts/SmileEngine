#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <cuda_d3d11_interop.h>

#include <algorithm>
#include <cmath>

namespace Smile
{
	namespace Raster
	{
		__host__ __device__ void FindAABB(const Triangle& triangle, glm::vec2& minPoint, glm::vec2& maxPoint)
		{
			minPoint.x = min(min(triangle.Vertex0.Position.x, triangle.Vertex1.Position.x), triangle.Vertex2.Position.x);
			minPoint.y = min(min(triangle.Vertex0.Position.y, triangle.Vertex1.Position.y), triangle.Vertex2.Position.y);

			maxPoint.x = max(max(triangle.Vertex0.Position.x, triangle.Vertex1.Position.x), triangle.Vertex2.Position.x);
			maxPoint.y = max(max(triangle.Vertex0.Position.y, triangle.Vertex1.Position.y), triangle.Vertex2.Position.y);
		}

		__host__ __device__ float CalculateSignedArea(const Triangle& triangle)
		{
			return 0.5f * ((triangle.Vertex2.Position.x - triangle.Vertex0.Position.x) * (triangle.Vertex1.Position.y - triangle.Vertex0.Position.y)
				- (triangle.Vertex1.Position.x - triangle.Vertex0.Position.x) * (triangle.Vertex2.Position.y - triangle.Vertex0.Position.y));
		}

		/*__host__ __device__ bool InsideOutsideCheck(const glm::vec3& barycentricCoord) 
		{
			return barycentricCoord.x >= 0.0f && barycentricCoord.x <= 1.0f 
				&& barycentricCoord.y >= 0.0f && barycentricCoord.y <= 1.0f 
				&& barycentricCoord.z >= 0.0f && barycentricCoord.z <= 1.0f;
		}*/

		__global__ void RasterizerKernel(Triangle* pTriangles, uint32_t triangleCount, float* pDepthBuffer, uint32_t* pDepth, uint32_t width, uint32_t height, uint8_t* pScreenBuffer, uint8_t colorChannelCount)
		{
			uint32_t triangleIndex = (blockIdx.x * blockDim.x) + threadIdx.x;

			if (triangleIndex < triangleCount)
			{
				Triangle& triangle = pTriangles[triangleIndex];

				for (uint32_t i{}; i < 3; ++i)
				{
					triangle.Vertices[i].Position.x = (triangle.Vertices[i].Position.x + 1.0f) * 0.5f * width;
					triangle.Vertices[i].Position.y = (1.0f - triangle.Vertices[i].Position.y) * 0.5f * height;
				}

				glm::vec2 boundingBoxMin{};
				glm::vec2 boundingBoxMax{};
				FindAABB(triangle, boundingBoxMin, boundingBoxMax);

				boundingBoxMin.x = max(boundingBoxMin.x, 0.0f);
				boundingBoxMin.y = max(boundingBoxMin.y, 0.0f);
				boundingBoxMax.x = min(boundingBoxMax.x, static_cast<float>(width));
				boundingBoxMax.y = min(boundingBoxMax.y, static_cast<float>(height));

				for (uint32_t y = static_cast<uint32_t>(std::floor(boundingBoxMin.y)); y < static_cast<uint32_t>(std::ceil(boundingBoxMax.y)); ++y)
				{
					for (uint32_t x = static_cast<uint32_t>(std::floor(boundingBoxMin.x)); x < static_cast<uint32_t>(std::ceil(boundingBoxMax.x)); ++x)
					{
						uint32_t pixelIndex = y * width + x;
						glm::vec2 pixel{ x, y };
						//glm::vec3 barycentricCoord = CalculateBarycentricCoordinate(triangle, pixel);

						// Calculate the edges of the triangle
						const glm::vec3 a{ triangle.Vertex1.Position - triangle.Vertex0.Position };
						const glm::vec3 b{ triangle.Vertex2.Position - triangle.Vertex1.Position };
						const glm::vec3 c{ triangle.Vertex0.Position - triangle.Vertex2.Position };

						// Get the vector from each vertex to the pixel
						const glm::vec2 ap{ pixel - glm::vec2{ triangle.Vertex0.Position } };
						const glm::vec2 bp{ pixel - glm::vec2{ triangle.Vertex1.Position } };
						const glm::vec2 cp{ pixel - glm::vec2{ triangle.Vertex2.Position } };

						// Get the cross product between each edge and the previous calculated vector
						const float crossA{ a.x * ap.y - a.y * ap.x };
						const float crossB{ b.x * bp.y - b.y * bp.x };
						const float crossC{ c.x * cp.y - c.y * cp.x };

						if (/*InsideOutsideCheck(barycentricCoord)*/ (crossA >= 0) && (crossB >= 0) && (crossC >= 0))
						{
							VS_INPUT vsInput{};
							//vsInput.Position = GetPositionAtCoordinate(barycentricCoord, pTriangles[triangleIndex]);

							// Lock until its our turn to do a compare
							while (!atomicCAS(&pDepth[pixelIndex], 0, 1))
								;

							if (vsInput.Position.z < pDepthBuffer[pixelIndex])
								pDepthBuffer[pixelIndex] = vsInput.Position.z;

							pScreenBuffer[pixelIndex * colorChannelCount] = 0;
							pScreenBuffer[pixelIndex * colorChannelCount + 1] = 0;
							pScreenBuffer[pixelIndex * colorChannelCount + 2] = 255.f;

							// Release lock
							pDepth[pixelIndex] = 0;
						}
					}
				}
			}
		}
	}
}