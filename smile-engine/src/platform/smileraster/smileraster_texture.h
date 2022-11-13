#pragma once
#include "smile_engine/graphic/resource/texture.h"
#include "smileraster_context.h"

#include <SDL_image.h>

namespace smile::graphic
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
        virtual Uint32 GetWidth() const override
        {
            return m_Width;
        }
        virtual Uint32 GetHeight() const override
        {
            return m_Height;
        }
        virtual void *GetData() const override
        {
            return reinterpret_cast< void * >( m_Texture );
        }

     private:
        std::string m_FilePath;
        Uint32 m_Width = 0;
        Uint32 m_Height = 0;

        SmileRasterContext *m_pSmileRasterContext = nullptr;
        Raster::TextureID m_Texture = SMR_INVALID_TEXTURE_ID;
        SDL_Surface *m_pSurface = nullptr;
        uint8_t *m_pPixels = nullptr;
    };
}
