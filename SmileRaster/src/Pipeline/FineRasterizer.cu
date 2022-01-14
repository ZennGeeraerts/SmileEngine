#include "DataStructs.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Smile
{
	namespace Raster
	{
		//void FineRasterizer(Bin* pBins, uint32_t binCountX, uint32_t binCountY, uint32_t binWidth, uint32_t binHeight, VertexShaderOutput* pPixelData, float* pDepthBuffer, uint32_t width)
		//{
		//	for (uint32_t binY{}; binY < binCountY; ++binY)
		//	{
		//		for (uint32_t binX{}; binX < binCountX; ++binX)
		//		{
		//			if ((binX <= binCountX) && (binY < binCountY))
		//			{
		//				Bin& bin{ pBins[binY * binCountX + binX] };

		//				uint32_t minX = binX * binWidth;
		//				uint32_t minY = binY * binHeight;
		//				uint32_t maxX = minX + binWidth;
		//				uint32_t maxY = minY + binHeight;

		//				for (uint32_t t{}; t < bin.TriangleCount; ++t)
		//				{
		//					Triangle& triangle{ bin.Triangles[t] };

		//					const glm::vec3 a{ triangle.Vertex1.Position - triangle.Vertex0.Position };
		//					const glm::vec3 b{ triangle.Vertex2.Position - triangle.Vertex1.Position };
		//					const glm::vec3 c{ triangle.Vertex0.Position - triangle.Vertex2.Position };

		//					for (uint32_t y{ minY }; y < maxY; ++y)
		//					{
		//						for (uint32_t x{ minX }; x < maxX; ++x)
		//						{
		//							uint32_t pixelIndex = y * width + x;
		//							glm::vec2 pixel{ x, y };

		//							// Get the vector from each vertex to the pixel
		//							const glm::vec2 ap{ pixel - glm::vec2{ triangle.Vertex0.Position } };
		//							const glm::vec2 bp{ pixel - glm::vec2{ triangle.Vertex1.Position } };
		//							const glm::vec2 cp{ pixel - glm::vec2{ triangle.Vertex2.Position } };

		//							// Get the cross product between each edge and the previous calculated vector
		//							const float crossA{ a.x * ap.y - a.y * ap.x };
		//							const float crossB{ b.x * bp.y - b.y * bp.x };
		//							const float crossC{ c.x * cp.y - c.y * cp.x };

		//							// Inside outside check
		//							if ((crossA >= 0) && (crossB >= 0) && (crossC >= 0))
		//							{
		//								const float area2{ a.x + b.y - a.y - b.x };
		//								const float weight0{ crossB / area2 };
		//								const float weight1{ crossC / area2 };
		//								const float weight2{ crossA / area2 };

		//								const float depthValue{ 1 / ((1 / triangle.Vertex0.Position.z * weight0) + (1 / triangle.Vertex1.Position.z * weight1) + (1 / triangle.Vertex2.Position.z * weight2)) };

		//								// Lock pixel

		//								// Depth test
		//								if (depthValue < pDepthBuffer[pixelIndex])
		//								{
		//									pDepthBuffer[pixelIndex] = depthValue;

		//									const float wValue{ 1 / ((1 / triangle.Vertex0.Position.w * weight0) + (1 / triangle.Vertex1.Position.w * weight1) + (1 / triangle.Vertex2.Position.w * weight2)) };
		//									pPixelData[pixelIndex].Position = { pixel.x, pixel.y, depthValue, wValue };
		//									pPixelData[pixelIndex].Color = ((triangle.Vertex0.Color / triangle.Vertex0.Position.w * weight0)
		//										+ (triangle.Vertex1.Color / triangle.Vertex1.Position.w * weight1)
		//										+ (triangle.Vertex2.Color / triangle.Vertex2.Position.w * weight2)) * wValue;
		//								}

		//								// Release lock
		//								//pPixelLock[pixelIndex] = 0;

		//								bin.TriangleCount = 0;
		//							}
		//						}
		//					}
		//				}
		//			}
		//		}
		//	}
		//}

		__global__ void FineRasterizerKernel(Bin* pBins, const Triangle* pTriangles, uint32_t binCountX, uint32_t binCountY, uint32_t binWidth, uint32_t binHeight, VertexShaderOutput* pPixelData, float* pDepthBuffer, uint32_t width)
		{
			uint32_t binX = (blockIdx.x * blockDim.x) + threadIdx.x;
			uint32_t binY = (blockIdx.y * blockDim.y) + threadIdx.y;

			if ((binX <= binCountX) && (binY < binCountY))
			{
				Bin& bin{ pBins[binY * binCountX + binX] };

				uint32_t minX = binX * binWidth;
				uint32_t minY = binY * binHeight;
				uint32_t maxX = minX + binWidth;
				uint32_t maxY = minY + binHeight;

				for (uint32_t t{}; t < bin.QueueSize; ++t)
				{
					const Triangle& triangle{ pTriangles[bin.Queue[t]] };

					const glm::vec3 a{ triangle.Vertex1.Position - triangle.Vertex0.Position };
					const glm::vec3 b{ triangle.Vertex2.Position - triangle.Vertex1.Position };
					const glm::vec3 c{ triangle.Vertex0.Position - triangle.Vertex2.Position };

					for (uint32_t y{ minY }; y < maxY; ++y)
					{
						for (uint32_t x{ minX }; x < maxX; ++x)
						{
							uint32_t pixelIndex = y * width + x;
							glm::vec2 pixel{ x, y };

							// Get the vector from each vertex to the pixel
							const glm::vec2 ap{ pixel - glm::vec2{ triangle.Vertex0.Position } };
							const glm::vec2 bp{ pixel - glm::vec2{ triangle.Vertex1.Position } };
							const glm::vec2 cp{ pixel - glm::vec2{ triangle.Vertex2.Position } };

							// Get the cross product between each edge and the previous calculated vector
							const float crossA{ a.x * ap.y - a.y * ap.x };
							const float crossB{ b.x * bp.y - b.y * bp.x };
							const float crossC{ c.x * cp.y - c.y * cp.x };

							// Inside outside check
							if ((crossA >= 0) && (crossB >= 0) && (crossC >= 0))
							{
								const float area2{ a.x + b.y - a.y - b.x };
								const float weight0{ crossB / area2 };
								const float weight1{ crossC / area2 };
								const float weight2{ crossA / area2 };

								const float depthValue{ 1 / ((1 / triangle.Vertex0.Position.z * weight0) + (1 / triangle.Vertex1.Position.z * weight1) + (1 / triangle.Vertex2.Position.z * weight2)) };

								// Depth test
								if (depthValue < pDepthBuffer[pixelIndex])
								{
									pDepthBuffer[pixelIndex] = depthValue;

									const float wValue{ 1 / ((1 / triangle.Vertex0.Position.w * weight0) + (1 / triangle.Vertex1.Position.w * weight1) + (1 / triangle.Vertex2.Position.w * weight2)) };
									pPixelData[pixelIndex].Position = { pixel.x, pixel.y, depthValue, wValue };
									pPixelData[pixelIndex].Color = ((triangle.Vertex0.Color / triangle.Vertex0.Position.w * weight0)
										+ (triangle.Vertex1.Color / triangle.Vertex1.Position.w * weight1)
										+ (triangle.Vertex2.Color / triangle.Vertex2.Position.w * weight2)) * wValue;
								}
							}
						}
					}
				}

				bin.QueueSize = 0;
			}
		}
	}
}