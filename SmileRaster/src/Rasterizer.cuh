#include "DataStructs.cuh"

namespace Smile
{
	namespace Raster
	{
		struct RenderConfig final
		{
			uint32_t BlockSize = 8;
			uint32_t BinSizeX = 64;
			uint32_t BinSizeY = 64;
		};

		class Rasterizer final
		{
		public:
			Rasterizer(const RenderConfig& renderCfg);
			~Rasterizer();

			void SetFramebuffer(Framebuffer* pFramebuffer);
			void Draw(uint32_t primitiveCount);

		private:
			RenderConfig m_RenderConfig;

			Framebuffer* m_pFramebuffer = nullptr;
			VertexBuffer* m_pVertexBuffer = nullptr;
			uint32_t m_VertexStride = 0;
			IndexBuffer* m_pIndexBuffer = nullptr;
			Triangle* d_PrimitiveBuffer = nullptr;
			ShaderData m_ShaderData{};

			Bin* d_Bins = nullptr;
			uint32_t m_BinWidth{};
			uint32_t m_BinHeight{};

			friend class DeviceContext;
		};
	}
}