#include <DirectXMath.h>

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
			bool SetVertexBuffer(BufferID id);

			BufferID CreateIndexBuffer(uint32_t* pIndices, uint32_t count);
			bool SetIndexBuffer(BufferID id);

			void Clear(const DirectX::XMFLOAT3& clearColor);
			void DrawIndexed();

			void Resize(uint32_t width, uint32_t height, uint8_t* pScreenBuffer);

		private:
			// GPU
			uint8_t* d_ScreenBuffer = nullptr;
			void* d_VertexBuffers[SMR_MAX_BUFFER_COUNT];
			uint32_t* d_IndexBuffers[SMR_MAX_BUFFER_COUNT];

			void* d_ActiveVertexBuffer = nullptr;
			uint32_t* d_ActiveIndexBuffer = nullptr;

			// CPU
			uint32_t m_VertexBufferCount = 0;
			uint32_t m_IndexBufferCount = 0;

			DeviceContextData m_DCData;
		};
	}
}