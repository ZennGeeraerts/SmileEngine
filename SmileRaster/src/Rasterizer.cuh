#include "DataStructs.cuh"

namespace Smile
{
	namespace Raster
	{
		struct RenderConfig final
		{
			uint32_t BlockSize = 1;
			uint32_t BinSizeX = 128;
			uint32_t BinSizeY = 128;
			uint32_t TileSizeX = 2;
			uint32_t TileSizeY = 2;
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
			Shader m_Shader{};

			Bin* d_Bins = nullptr;
			Bin* d_Tiles = nullptr;
			uint32_t m_BinWidth{};
			uint32_t m_BinHeight{};
			uint32_t m_TileWidth{};
			uint32_t m_TileHeight{};

			friend class DeviceContext;
		};
	}
}