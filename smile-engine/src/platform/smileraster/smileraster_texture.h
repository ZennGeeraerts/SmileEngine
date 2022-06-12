#pragma once
#include "smile_engine/renderer/resource/texture.h"
#include "smileraster_context.h"

#include <SDL_image.h>

namespace smile::renderer
{
    class SmileRasterTexture2D final : public Texture2D
    {
     public:
        SmileRasterTexture2D( const std::string &file_path );
        virtual ~SmileRasterTexture2D();

        virtual const std::string &getFilePath() const override
        {
            return filePath;
        };
        virtual Uint32 getWidth() const override
        {
            return width;
        }
        virtual Uint32 getHeight() const override
        {
            return height;
        }
        virtual void *getData() const override
        {
            return reinterpret_cast< void * >( texture );
        }

     private:
        std::string filePath;
        Uint32 width = 0;
        Uint32 height = 0;

        SmileRasterContext *smileRasterContext = nullptr;
        Raster::TextureID texture = SMR_INVALID_TEXTURE_ID;
        SDL_Surface *surface = nullptr;
        uint8_t *pixels = nullptr;
    };
}
