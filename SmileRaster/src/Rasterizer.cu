#include "Rasterizer.cuh"

#include "Utils.cuh"

// Pipeline
#include "Pipeline/InputAssembler.cu"
#include "Pipeline/VertexShader.cu"
#include "Pipeline/PrimitiveAssembler.cu"
#include "Pipeline/BinRasterizer.cu"
#include "Pipeline/FineRasterizer.cu"
#include "Pipeline/PixelShader.cu"

#include <iostream>

namespace Smile
{
	namespace Raster
	{
		Rasterizer::Rasterizer(const RenderConfig& renderCfg)
			: m_RenderConfig{ renderCfg }
		{
			size_t size{ sizeof(Bin) * renderCfg.BinSizeX * renderCfg.BinSizeY };
			GPU_ERROR_CHECK(cudaMalloc(&d_Bins, size));

			size = sizeof(Bin) * renderCfg.BinSizeX * renderCfg.TileSizeX * renderCfg.BinSizeX * renderCfg.TileSizeY;
			GPU_ERROR_CHECK(cudaMalloc(&d_Tiles, size));
		}

		Rasterizer::~Rasterizer()
		{
			GPU_ERROR_CHECK(cudaFree(d_Bins));
			GPU_ERROR_CHECK(cudaFree(d_Tiles));
		}

		void Rasterizer::SetFramebuffer(Framebuffer* pFramebuffer)
		{
			if (!m_pFramebuffer || ((m_pFramebuffer->Width != pFramebuffer->Width) || (m_pFramebuffer->Height != pFramebuffer->Height)))
			{
				m_BinWidth = { static_cast<uint32_t>(ceil(static_cast<float>(pFramebuffer->Width) / m_RenderConfig.BinSizeX)) };
				m_BinHeight = { static_cast<uint32_t>(ceil(static_cast<float>(pFramebuffer->Height) / m_RenderConfig.BinSizeY)) };
				m_TileWidth = m_BinWidth / m_RenderConfig.TileSizeX;
				m_TileHeight = m_BinWidth / m_RenderConfig.TileSizeY;
			}

			m_pFramebuffer = pFramebuffer;
		}

		/*__global__ void ClearBinsKernel(Bin* d_Bins, uint32_t binCountX, uint32_t binCountY)
		{
			uint32_t binX = (blockIdx.x * blockDim.x) + threadIdx.x;
			uint32_t binY = (blockIdx.y * blockDim.y) + threadIdx.y;

			if ((binX < binCountX) && (binY < binCountY))
			{
				d_Bins[binY * binCountX + binCountY].QueueSize = 0;
			}
		}*/

		void Rasterizer::Draw(uint32_t primitiveCount)
		{
			assert(m_pFramebuffer != nullptr);
			assert(m_pVertexBuffer != nullptr);
			assert(m_pIndexBuffer != nullptr);

			uint32_t vertexCount = static_cast<uint32_t>(ceil(m_pVertexBuffer->ByteWidth / static_cast<float>(m_VertexStride)));
			dim3 blockSize = { m_RenderConfig.BlockSize, m_RenderConfig.BlockSize };

			// Input assembler
			dim3 gridSize = static_cast<uint32_t>(ceil(vertexCount / static_cast<float>(m_RenderConfig.BlockSize)));
			InputAssemblerKernel << <gridSize, blockSize >> > (m_pVertexBuffer->d_Vertices, vertexCount, m_pVertexBuffer->d_VertexShaderInput, m_VertexStride);
			GPU_ERROR_CHECK(cudaDeviceSynchronize());

			// VertexShader
			VertexShaderKernel << <gridSize, blockSize >> > (m_pVertexBuffer->d_VertexShaderInput, m_pVertexBuffer->d_VertexShaderOutput, vertexCount, m_Shader.Mat4Data["ViewProjection"], m_Shader.Mat4Data["World"]);
			GPU_ERROR_CHECK(cudaDeviceSynchronize());

			// Primitive assembler	
			gridSize = static_cast<uint32_t>(ceil(primitiveCount / static_cast<float>(m_RenderConfig.BlockSize)));
			PrimitiveAssemblerKernel << <gridSize, blockSize >> > (d_PrimitiveBuffer, primitiveCount, m_pVertexBuffer->d_VertexShaderOutput, m_pIndexBuffer->d_Indices);
			GPU_ERROR_CHECK(cudaDeviceSynchronize());

			// Bin Rasterizer
			BinRasterizerKernel << <gridSize, blockSize >> > (d_PrimitiveBuffer, primitiveCount, d_Bins, m_RenderConfig.BinSizeX, m_RenderConfig.BinSizeY,
				m_BinWidth, m_BinHeight, m_pFramebuffer->Width, m_pFramebuffer->Height);
			GPU_ERROR_CHECK(cudaDeviceSynchronize());

			/*Bin* pBins = (Bin*)malloc(sizeof(Bin) * m_RenderConfig.BinSizeX * m_RenderConfig.BinSizeY);
			cudaMemcpy(pBins, d_Bins, sizeof(Bin) * m_RenderConfig.BinSizeX * m_RenderConfig.BinSizeY, cudaMemcpyDeviceToHost);
			for (uint32_t y{}; y < m_RenderConfig.BinSizeY; ++y)
			{
				for (uint32_t x{}; x < m_RenderConfig.BinSizeX; ++x)
				{
					std::cout << pBins[y * m_RenderConfig.BinSizeX + x].QueueSize / sizeof(unsigned long long int);
				}

				std::cout << "\n";
			}
			free(pBins);*/

			// Coarse Rasterizer
			/*gridSize = { static_cast<uint32_t>(ceil(m_RenderConfig.BinSizeX / static_cast<float>(m_RenderConfig.BlockSize))),
			static_cast<uint32_t>(ceil(m_RenderConfig.BinSizeY / static_cast<float>(m_RenderConfig.BlockSize))) };
			CoarseRasterizerKernel << <gridSize, blockSize >> > (d_Bins, d_Tiles, d_PrimitiveBuffer, m_RenderConfig.BinSizeX, m_RenderConfig.BinSizeY, m_RenderConfig.TileSizeX, m_RenderConfig.TileSizeY, m_TileWidth, m_TileHeight);
			GPU_ERROR_CHECK(cudaDeviceSynchronize());*/

			/*Bin* pTiles = (Bin*)malloc(sizeof(Bin) * m_RenderConfig.BinSizeX * m_RenderConfig.TileSizeX * m_RenderConfig.BinSizeY * m_RenderConfig.TileSizeY);
			cudaMemcpy(pTiles, d_Tiles, sizeof(Bin) * m_RenderConfig.BinSizeX * m_RenderConfig.TileSizeX * m_RenderConfig.BinSizeY * m_RenderConfig.TileSizeY, cudaMemcpyDeviceToHost);
			for (uint32_t y{}; y < m_RenderConfig.BinSizeY; ++y)
			{
				for (uint32_t x{}; x < m_RenderConfig.BinSizeX; ++x)
				{
					std::cout << pTiles[y * 4 * (m_RenderConfig.BinSizeX * m_RenderConfig.TileSizeX) + x * 4].QueueSize / sizeof(unsigned long long int);
				}

				std::cout << "\n";
			}
			free(pTiles);*/

			// Fine Rasterizer
			gridSize = { static_cast<uint32_t>(ceil(m_RenderConfig.BinSizeX /** m_RenderConfig.TileSizeX*/ / static_cast<float>(m_RenderConfig.BlockSize))),
				static_cast<uint32_t>(ceil(m_RenderConfig.BinSizeY /** m_RenderConfig.TileSizeY*/ / static_cast<float>(m_RenderConfig.BlockSize))) };
			FineRasterizerKernel << <gridSize, blockSize >> > (d_Bins, d_PrimitiveBuffer, m_RenderConfig.BinSizeX/* * m_RenderConfig.TileSizeX*/, m_RenderConfig.BinSizeY /** m_RenderConfig.TileSizeY*/, m_BinWidth, m_BinHeight, m_pFramebuffer->d_PixelData, m_pFramebuffer->d_DepthBuffer, m_pFramebuffer->Width);
			GPU_ERROR_CHECK(cudaDeviceSynchronize());

			/*gridSize = { static_cast<uint32_t>(ceil(m_RenderConfig.BinSizeX / static_cast<float>(m_RenderConfig.BlockSize))),
				static_cast<uint32_t>(ceil(m_RenderConfig.BinSizeY / static_cast<float>(m_RenderConfig.BlockSize))) };
			ClearBinsKernel << <gridSize, blockSize >> > (d_Bins, m_RenderConfig.BinSizeX, m_RenderConfig.BinSizeY);*/

			/*Bin* pBins = (Bin*)malloc(sizeof(Bin) * m_RenderConfig.BinSizeX * m_RenderConfig.TileSizeX * m_RenderConfig.BinSizeY * m_RenderConfig.TileSizeY);
			cudaMemcpy(pBins, d_Tiles, sizeof(Bin) * m_RenderConfig.BinSizeX * m_RenderConfig.TileSizeX * m_RenderConfig.BinSizeY * m_RenderConfig.TileSizeY, cudaMemcpyDeviceToHost);
			Triangle* pTriangles = (Triangle*)malloc(sizeof(Triangle) * primitiveCount);
			cudaMemcpy(pTriangles, d_PrimitiveBuffer, sizeof(Triangle) * primitiveCount, cudaMemcpyDeviceToHost);
			InterpolatedAttributes* pPixelData = (InterpolatedAttributes*)malloc(sizeof(InterpolatedAttributes) * m_pFramebuffer->Width * m_pFramebuffer->Height);
			float* pDepthBuffer = (float*)malloc(sizeof(float) * m_pFramebuffer->Width * m_pFramebuffer->Height);
			FineRasterizer(pBins, pTriangles, m_RenderConfig.BinSizeX * m_RenderConfig.TileSizeX, m_RenderConfig.BinSizeY * m_RenderConfig.TileSizeY, m_TileWidth, m_TileHeight, pPixelData, pDepthBuffer, m_pFramebuffer->Width);
			free(pBins);
			free(pTriangles);
			free(pPixelData);
			free(pDepthBuffer);*/

			/*VertexShaderOutput* pOutput = (VertexShaderOutput*)malloc(sizeof(VertexShaderOutput) * m_pFramebuffer->Width * m_pFramebuffer->Height);
			GPU_ERROR_CHECK(cudaMemcpy(pOutput, m_pFramebuffer->d_PixelData, sizeof(VertexShaderOutput) * m_pFramebuffer->Width * m_pFramebuffer->Height, cudaMemcpyDeviceToHost));
			for (uint32_t y{}; y < m_pFramebuffer->Height; ++y)
			{
				for (uint32_t x{}; x < m_pFramebuffer->Width; ++x)
				{
					if (glm::length(pOutput[y * m_pFramebuffer->Width + x].Normal) > 0.0f)
						std::cout << pOutput[y * m_pFramebuffer->Width + x].Normal.x << ", " << pOutput[y * m_pFramebuffer->Width + x].Normal.y << ", " << pOutput[y * m_pFramebuffer->Width + x].Normal.z << '\n';
				}
			}
			free(pOutput);*/

			// Pixel Shader
			gridSize = { static_cast<uint32_t>(ceil(m_pFramebuffer->Width / static_cast<float>(m_RenderConfig.BlockSize))),
								static_cast<uint32_t>(ceil(m_pFramebuffer->Height / static_cast<float>(m_RenderConfig.BlockSize))) };
			PixelShaderKernel << <gridSize, blockSize >> > (*m_pFramebuffer, m_Shader.Texture2DData["AlbedoMap"]);
			GPU_ERROR_CHECK(cudaDeviceSynchronize());

			// Copy data to host buffer
			size_t size = sizeof(uint8_t) * m_pFramebuffer->ColorChannelCount * m_pFramebuffer->Width * m_pFramebuffer->Height;
			GPU_ERROR_CHECK(cudaMemcpy(m_pFramebuffer->pHostOutput, m_pFramebuffer->d_ColorBuffer, size, cudaMemcpyDeviceToHost));
		}
	}
}