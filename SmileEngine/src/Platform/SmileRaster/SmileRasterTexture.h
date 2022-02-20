#pragma once
#include "SmileEngine/Renderer/Texture.h"
#include "SmileRasterContext.h"

#include <SDL_image.h>

namespace smile
{
    class SmileRasterTexture2D final : public Texture2D
    {
      public:
        SmileRasterTexture2D( const std::string &filePath );
        virtual ~SmileRasterTexture2D();

        virtual const std::string &GetFilePath() const override
        {
            return m_FilePath;
        };
        virtual uint32_t GetWidth() const override
        {
            return m_Width;
        }
        virtual uint32_t GetHeight() const override
        {
            return m_Height;
        }
        virtual void *GetData() const override
        {
            return reinterpret_cast< void * >( m_Texture );
        }

      private:
        std::string m_FilePath;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

        SmileRasterContext *m_pSmileRasterContext = nullptr;
        Raster::TextureID m_Texture = SMR_INVALID_TEXTURE_ID;
        SDL_Surface *m_pSurface = nullptr;
        uint8_t *m_pPixels = nullptr;
    };
}
