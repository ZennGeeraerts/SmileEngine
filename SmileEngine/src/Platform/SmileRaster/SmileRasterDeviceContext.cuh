#include <stdint.h>

namespace Smile
{
	struct SmileRasterDeviceContext final
	{
	public:
		SmileRasterDeviceContext(uint32_t width, uint32_t height);
		~SmileRasterDeviceContext();

		void SetVertexBuffer(void* pVertexBuffer, uint32_t count, uint32_t stride);
		void SetIndexBuffer(uint32_t* pIndexBuffer, uint32_t count);

		void DrawIndexed();

	public:
		uint32_t h_Width;
		uint32_t h_Height;

		void* d_VertexBuffer = nullptr;
		uint32_t h_VertexBufferCount;
		uint32_t h_VertexBufferStride;

		uint32_t* d_IndexBuffer = nullptr;
		uint32_t h_IndexBufferCount;
	};
}