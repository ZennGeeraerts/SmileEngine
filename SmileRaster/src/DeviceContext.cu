#include "DeviceContext.cuh"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "Utils.cuh"

namespace Smile
{
	namespace Raster
	{
		DeviceContext::DeviceContext(const RenderConfig& renderCfg)
			: m_pRasterizer{ new Rasterizer{ renderCfg } }
		{
			
		}

		DeviceContext::~DeviceContext()
		{
			delete m_pRasterizer;

			for (BufferID i{}; i < m_FramebufferCount; ++i)
			{
				GPU_ERROR_CHECK(cudaFree(m_Framebuffers[i].d_Colorbuffer));
				GPU_ERROR_CHECK(cudaFree(m_Framebuffers[i].d_Depthbuffer));
				GPU_ERROR_CHECK(cudaFree(m_Framebuffers[i].d_PixelData));
			}

			for (BufferID i{}; i < m_VertexBufferCount; ++i)
			{
				GPU_ERROR_CHECK(cudaFree(m_VertexBuffers[i].d_Vertices));
				GPU_ERROR_CHECK(cudaFree(m_VertexBuffers[i].d_VertexShaderOutput));
			}

			for (BufferID i{}; i < m_IndexBufferCount; ++i)
			{
				GPU_ERROR_CHECK(cudaFree(m_IndexBuffers[i].d_Indices));
				GPU_ERROR_CHECK(cudaFree(d_PrimitiveBuffers[i]));
			}
		}

		// Buffer creation
		BufferID DeviceContext::CreateFramebuffer(uint8_t* pBuffer, uint32_t width, uint32_t height, ColorbufferFormat colorFormat)
		{
			if (m_FramebufferCount < SMR_MAX_BUFFER_COUNT)
			{
				Framebuffer& framebuffer = m_Framebuffers[m_FramebufferCount];
				switch (colorFormat)
				{
				case ColorbufferFormat::eRGB:
					framebuffer.ColorChannelCount = 3;
					break;

				case ColorbufferFormat::eRGBA:
					framebuffer.ColorChannelCount = 4;
					break;
				}

				framebuffer.Width = width;
				framebuffer.Height = height;
				framebuffer.pOutput = pBuffer;

				size_t size = sizeof(uint8_t) * framebuffer.ColorChannelCount * width * height;
				GPU_ERROR_CHECK(cudaMalloc(&framebuffer.d_Colorbuffer, size));

				size = sizeof(float) * width * height;
				GPU_ERROR_CHECK(cudaMalloc(&framebuffer.d_Depthbuffer, size));

				size = sizeof(InterpolatedAttributes) * width * height;
				GPU_ERROR_CHECK(cudaMalloc(&framebuffer.d_PixelData, size));

				++m_FramebufferCount;
				return m_FramebufferCount - 1;
			}

			return SMR_INVALID_BUFFER_ID;
		}

		BufferID DeviceContext::CreateVertexBuffer(void* pVertices, uint32_t count, uint32_t byteWidth)
		{
			if (m_VertexBufferCount < SMR_MAX_BUFFER_COUNT)
			{
				VertexBuffer& vertexBuffer = m_VertexBuffers[m_VertexBufferCount];

				GPU_ERROR_CHECK(cudaMalloc(&vertexBuffer.d_Vertices, byteWidth));
				GPU_ERROR_CHECK(cudaMemcpy(vertexBuffer.d_Vertices, pVertices, byteWidth, cudaMemcpyHostToDevice));
				vertexBuffer.ByteWidth = byteWidth;

				size_t size = sizeof(VertexShaderOutput) * count;
				GPU_ERROR_CHECK(cudaMalloc(&vertexBuffer.d_VertexShaderOutput, size));

				++m_VertexBufferCount;

				return m_VertexBufferCount - 1;
			}

			return SMR_INVALID_BUFFER_ID;
		}

		BufferID DeviceContext::CreateIndexBuffer(uint32_t* pIndices, uint32_t count)
		{
			if (m_IndexBufferCount < SMR_MAX_BUFFER_COUNT)
			{
				size_t size = sizeof(uint32_t) * count;
				GPU_ERROR_CHECK(cudaMalloc(&m_IndexBuffers[m_IndexBufferCount].d_Indices, size));
				GPU_ERROR_CHECK(cudaMemcpy(m_IndexBuffers[m_IndexBufferCount].d_Indices, pIndices, size, cudaMemcpyHostToDevice));

				size = sizeof(Triangle) * count / 3;
				GPU_ERROR_CHECK(cudaMalloc(&d_PrimitiveBuffers[m_IndexBufferCount], size));

				++m_IndexBufferCount;

				return m_IndexBufferCount - 1;
			}

			return SMR_INVALID_BUFFER_ID;
		}

		bool DeviceContext::BindFramebuffer(BufferID id)
		{
			if ((id < static_cast<int>(m_FramebufferCount)) && (id >= SMR_INVALID_BUFFER_ID))
			{
				m_pRasterizer->SetFramebuffer(&m_Framebuffers[id]);
				return true;
			}

			m_pRasterizer->m_pFramebuffer = nullptr;
			return false;
		}

		bool DeviceContext::BindVertexBuffer(BufferID id, uint32_t stride)
		{
			if ((id < static_cast<int>(m_VertexBufferCount)) && (id >= SMR_INVALID_BUFFER_ID))
			{
				m_pRasterizer->m_pVertexBuffer = &m_VertexBuffers[id];
				m_pRasterizer->m_VertexStride = stride;
				return true;
			}

			m_pRasterizer->m_pVertexBuffer = nullptr;
			return false;
		}

		bool DeviceContext::BindIndexBuffer(BufferID id)
		{
			if ((id < static_cast<int>(m_IndexBufferCount)) && (id >= SMR_INVALID_BUFFER_ID))
			{
				m_pRasterizer->m_pIndexBuffer = &m_IndexBuffers[id];
				m_pRasterizer->d_PrimitiveBuffer = d_PrimitiveBuffers[id];
				return true;
			}

			m_pRasterizer->m_pIndexBuffer = nullptr;
			return false;
		}

		// Clearing
		__global__ void ClearFramebufferKernel(Framebuffer framebuffer, DirectX::XMFLOAT4 clearColor, bool bClearDepth)
		{
			uint32_t pixelX = (blockIdx.x * blockDim.x) + threadIdx.x;
			uint32_t pixelY = (blockIdx.y * blockDim.y) + threadIdx.y;
			uint32_t bufferIndex = pixelY * framebuffer.Width + pixelX;

			if (bufferIndex < (framebuffer.Width * framebuffer.Height))
			{
				framebuffer.d_Colorbuffer[bufferIndex * framebuffer.ColorChannelCount] = static_cast<uint8_t>(clearColor.z * 255.f);
				framebuffer.d_Colorbuffer[bufferIndex * framebuffer.ColorChannelCount + 1] = static_cast<uint8_t>(clearColor.y * 255.f);
				framebuffer.d_Colorbuffer[bufferIndex * framebuffer.ColorChannelCount + 2] = static_cast<uint8_t>(clearColor.x * 255.f);

				if (framebuffer.ColorChannelCount > 3)
					framebuffer.d_Colorbuffer[bufferIndex * framebuffer.ColorChannelCount + 3] = static_cast<uint8_t>(clearColor.w * 255.f);

				if (bClearDepth)
					framebuffer.d_Depthbuffer[bufferIndex] = FLT_MAX;
			}
		}

		void DeviceContext::Clear(BufferID framebufferID, const DirectX::XMFLOAT4& clearColor, bool bClearDepth)
		{
			Framebuffer& framebuffer = m_Framebuffers[framebufferID];

			dim3 blockSize = { m_pRasterizer->m_RenderConfig.BlockSize, m_pRasterizer->m_RenderConfig.BlockSize };
			dim3 gridSize = { static_cast<uint32_t>(ceil(framebuffer.Width / static_cast<float>(m_pRasterizer->m_RenderConfig.BlockSize))),
								static_cast<uint32_t>(ceil(framebuffer.Height / static_cast<float>(m_pRasterizer->m_RenderConfig.BlockSize))) };

			ClearFramebufferKernel<<<gridSize, blockSize>>>(framebuffer, clearColor, bClearDepth);
		}

		// Rendering
		void DeviceContext::DrawIndexed(uint32_t indexCount)
		{
			assert(indexCount % 3 == 0);

			m_pRasterizer->Draw(indexCount / 3);
		}

		void DeviceContext::Resize(uint32_t width, uint32_t height, uint8_t* pScreenBuffer)
		{
			// Update device context data
			//m_DCData.Width = width;
			//m_DCData.Height = height;
			//m_DCData.pScreenBuffer = pScreenBuffer;

			//// Reallocate the screen buffer in device memory
			//GPU_ERROR_CHECK(cudaFree(d_ScreenBuffer));
			//size_t size = sizeof(uint8_t) * m_DCData.ColorChannelCount * width * height;
			//GPU_ERROR_CHECK(cudaMalloc(&d_ScreenBuffer, size));
			//GPU_ERROR_CHECK(cudaMemcpy(d_ScreenBuffer, pScreenBuffer, size, cudaMemcpyHostToDevice));

			//// Reallocate the depth buffer in device memory
			//GPU_ERROR_CHECK(cudaFree(d_DepthBuffer));
			//size = sizeof(float) * width * height;
			//GPU_ERROR_CHECK(cudaMalloc(&d_DepthBuffer, size));

			//GPU_ERROR_CHECK(cudaFree(d_PixelLock));
			//GPU_ERROR_CHECK(cudaMalloc(&d_PixelLock, sizeof(uint32_t) * width * height * m_DCData.ColorChannelCount));
		}

		void DeviceContext::SetShaderData(const DirectX::XMFLOAT4X4& viewProjection, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4X4& viewInverse)
		{
			/*GPU_ERROR_CHECK(cudaMemcpy(&d_ShaderData->ViewProjection, &viewProjection, sizeof(DirectX::XMFLOAT4X4), cudaMemcpyHostToDevice));
			GPU_ERROR_CHECK(cudaMemcpy(&d_ShaderData->World, &world, sizeof(DirectX::XMFLOAT4X4), cudaMemcpyHostToDevice));
			GPU_ERROR_CHECK(cudaMemcpy(&d_ShaderData->ViewInverse, &viewInverse, sizeof(DirectX::XMFLOAT4X4), cudaMemcpyHostToDevice));*/

			m_pRasterizer->m_ShaderData.ViewProjection = ConvertToGLMMat(viewProjection);
			m_pRasterizer->m_ShaderData.World = ConvertToGLMMat(world);
			m_pRasterizer->m_ShaderData.ViewInverse = ConvertToGLMMat(viewInverse);
		}
	}
}