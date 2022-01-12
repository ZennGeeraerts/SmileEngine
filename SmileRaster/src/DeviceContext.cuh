#include "DataStructs.cuh"

namespace Smile
{
	namespace Raster
	{
		#define SMR_MAX_BUFFER_COUNT 10
		#define SMR_INVALID_BUFFER_ID -1

		#define SMR_BIN_COUNT_X 64
		#define SMR_BIN_COUNT_Y 64
		#define SMR_TILE_COUNT_X 8
		#define SMR_TILE_COUNT_Y 8

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

			BufferID CreateVertexBuffer(void* pVertices, uint32_t count, uint32_t stride);
			BufferID CreateIndexBuffer(uint32_t* pIndices, uint32_t count);

			bool SetVertexBuffer(BufferID id);
			bool SetIndexBuffer(BufferID id);

			void Clear(const DirectX::XMFLOAT3& clearColor);
			void DrawIndexed();

			void Resize(uint32_t width, uint32_t height, uint8_t* pScreenBuffer);
			
			void SetShaderData(const DirectX::XMFLOAT4X4& viewProjection, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4X4& viewInverse);

		private:
			// GPU
			uint8_t* d_ScreenBuffer = nullptr;
			float* d_DepthBuffer = nullptr;
			VS_OUTPUT* d_PixelData = nullptr;

			VertexBuffer m_VertexBuffers[SMR_MAX_BUFFER_COUNT];
			VS_OUTPUT* d_VertexShaderOutputs[SMR_MAX_BUFFER_COUNT];
			IndexBuffer m_IndexBuffers[SMR_MAX_BUFFER_COUNT];
			Triangle* d_PrimitiveBuffers[SMR_MAX_BUFFER_COUNT];

			uint32_t* d_PixelLock = nullptr;
			
			Segment* d_Bins = nullptr;

			// CPU
			uint32_t m_VertexBufferCount = 0;
			uint32_t m_IndexBufferCount = 0;

			DeviceContextData m_DCData;

			BufferID m_ActiveVertexBufferID = SMR_INVALID_BUFFER_ID;
			BufferID m_ActiveIndexBufferID = SMR_INVALID_BUFFER_ID;

			ShaderData m_ShaderData{};

			uint32_t m_BinWidth{};
			uint32_t m_BinHeight{};
		};
	}
}