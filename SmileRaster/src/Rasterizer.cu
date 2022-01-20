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
		}

		Rasterizer::~Rasterizer()
		{
			GPU_ERROR_CHECK(cudaFree(d_Bins));
		}

		void Rasterizer::SetFramebuffer(Framebuffer* pFramebuffer)
		{
			if (!m_pFramebuffer || ((m_pFramebuffer->Width != pFramebuffer->Width) || (m_pFramebuffer->Height != pFramebuffer->Height)))
			{
				m_BinWidth = { static_cast<uint32_t>(ceil(static_cast<float>(pFramebuffer->Width) / m_RenderConfig.BinSizeX)) };
				m_BinHeight = { static_cast<uint32_t>(ceil(static_cast<float>(pFramebuffer->Height) / m_RenderConfig.BinSizeY)) };
			}

			m_pFramebuffer = pFramebuffer;
		}

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

			// Fine Rasterizer
			gridSize = { static_cast<uint32_t>(ceil(m_RenderConfig.BinSizeX / static_cast<float>(m_RenderConfig.BlockSize))),
				static_cast<uint32_t>(ceil(m_RenderConfig.BinSizeY / static_cast<float>(m_RenderConfig.BlockSize))) };
			FineRasterizerKernel << <gridSize, blockSize >> > (d_Bins, d_PrimitiveBuffer, m_RenderConfig.BinSizeX, m_RenderConfig.BinSizeY, m_BinWidth, m_BinHeight, m_pFramebuffer->d_PixelData, m_pFramebuffer->d_DepthBuffer, m_pFramebuffer->Width);
			GPU_ERROR_CHECK(cudaDeviceSynchronize());

			/*Bin* pBins = (Bin*)malloc(sizeof(Bin) * m_RenderConfig.BinSizeX * m_RenderConfig.BinSizeY);
			cudaMemcpy(pBins, d_Bins, sizeof(Bin) * m_RenderConfig.BinSizeX * m_RenderConfig.BinSizeY, cudaMemcpyDeviceToHost);
			Triangle* pTriangles = (Triangle*)malloc(sizeof(Triangle) * primitiveCount);
			cudaMemcpy(pTriangles, d_PrimitiveBuffer, sizeof(Triangle) * primitiveCount, cudaMemcpyDeviceToHost);
			InterpolatedAttributes* pPixelData = (InterpolatedAttributes*)malloc(sizeof(InterpolatedAttributes) * m_pFramebuffer->Width * m_pFramebuffer->Height);
			float* pDepthBuffer = (float*)malloc(sizeof(float) * m_pFramebuffer->Width * m_pFramebuffer->Height);
			FineRasterizer(pBins, pTriangles, m_RenderConfig.BinSizeX, m_RenderConfig.BinSizeY, m_BinWidth, m_BinHeight, pPixelData, pDepthBuffer, m_pFramebuffer->Width);
			free(pBins);
			free(pTriangles);
			free(pPixelData);
			free(pDepthBuffer);*/

			/*VS_OUTPUT* pOutput = (VS_OUTPUT*)malloc(sizeof(VS_OUTPUT) * m_DCData.Width * m_DCData.Height);
			GPU_ERROR_CHECK(cudaMemcpy(pOutput, d_PixelData, sizeof(VS_OUTPUT) * m_DCData.Width * m_DCData.Height, cudaMemcpyDeviceToHost));
			for (uint32_t y{}; y < m_DCData.Height / 10; ++y)
			{
				for (uint32_t x{}; x < m_DCData.Width / 10; ++x)
				{
					if (glm::length(pOutput[y * 10 * m_DCData.Width + x * 10].Color) > 0.0f)
						std::cout << '1';
					else
						std::cout << '0';
				}
				std::cout << '\n';
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