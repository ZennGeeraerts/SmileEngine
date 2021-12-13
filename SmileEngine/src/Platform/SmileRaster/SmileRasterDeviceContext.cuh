#include "SmileRasterUtils.cuh"

#include <stdint.h>
#include <vector>
#include <DirectXMath.h>

#define MAX_BUFFER_COUNT 10
#define INVALID_SM_BUFFER_ID -1

namespace Smile
{
	typedef int SMBufferID;

	struct SmileRasterDeviceContextData final
	{
		uint8_t* pScreenBuffer = nullptr;
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint8_t ColorChannelCount = 3;

		uint32_t TileSize = 8;
	};

	class SmileRasterDeviceContext final
	{
	public:
		SmileRasterDeviceContext(const SmileRasterDeviceContextData& data);
		~SmileRasterDeviceContext();

		uint8_t* GetScreenBuffer() const { return d_ScreenBuffer; }

		SMBufferID CreateVertexBuffer(void* pVertices, uint32_t count, uint32_t stride);
		bool SetVertexBuffer(SMBufferID id);

		SMBufferID CreateIndexBuffer(uint32_t* pIndices, uint32_t count);
		bool SetIndexBuffer(SMBufferID id);

		void Clear(const DirectX::XMFLOAT3& clearColor);
		void DrawIndexed();

	private:
		// GPU
		uint8_t* d_ScreenBuffer = nullptr;
		void* d_VertexBuffers[MAX_BUFFER_COUNT];
		uint32_t* d_IndexBuffers[MAX_BUFFER_COUNT];

		void* d_ActiveVertexBuffer = nullptr;
		uint32_t* d_ActiveIndexBuffer = nullptr;

		// CPU
		uint32_t m_VertexBufferCount = 0;
		uint32_t m_IndexBufferCount = 0;

		SmileRasterDeviceContextData m_DCData;
	};
}