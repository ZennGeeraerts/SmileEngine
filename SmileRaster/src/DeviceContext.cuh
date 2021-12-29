#include "DataStructs.cuh"

namespace Smile
{
	namespace Raster
	{
		#define SMR_MAX_BUFFER_COUNT 10
		#define SMR_INVALID_BUFFER_ID -1

		typedef int BufferID;

		struct DeviceContextData final
		{
			uint8_t* pScreenBuffer = nullptr;
			uint32_t Width = 0;
			uint32_t Height = 0;
			uint8_t ColorChannelCount = 3;

			uint32_t TileSize = 8;
		};

		class DeviceContext final
		{
		public:
			DeviceContext(const DeviceContextData& data);
			~DeviceContext();

			uint8_t* GetScreenBuffer() const { return d_ScreenBuffer; }

			BufferID CreateVertexBuffer(void* pVertices, uint32_t count, uint32_t stride);
			BufferID CreateIndexBuffer(uint32_t* pIndices, uint32_t count);

			bool SetVertexBuffer(BufferID id);
			bool SetIndexBuffer(BufferID id);

			void Clear(const DirectX::XMFLOAT3& clearColor);
			void DrawIndexed();

			void Resize(uint32_t width, uint32_t height, uint8_t* pScreenBuffer);
			
			void SetShaderData(DirectX::XMFLOAT4X4 viewProjection, DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 viewInverse);

		private:
			// GPU
			uint8_t* d_ScreenBuffer = nullptr;
			float* d_DepthBuffer = nullptr;

			VertexBuffer m_VertexBuffers[SMR_MAX_BUFFER_COUNT];
			IndexBuffer m_IndexBuffers[SMR_MAX_BUFFER_COUNT];
			ShaderData* d_ShaderData = nullptr;

			// CPU
			uint32_t m_VertexBufferCount = 0;
			uint32_t m_IndexBufferCount = 0;

			DeviceContextData m_DCData;

			BufferID m_ActiveVertexBufferID = SMR_INVALID_BUFFER_ID;
			BufferID m_ActiveIndexBufferID = SMR_INVALID_BUFFER_ID;
		};
	}
}