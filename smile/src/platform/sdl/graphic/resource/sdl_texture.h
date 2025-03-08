/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/graphic/resource/texture.h"
#include "platform/smileraster/graphic/smileraster_context.h"

#include <SDL_image.h>

namespace smile::graphic
{
    struct SDLTexture2D final : public Texture2D
    {
        SDLTexture2D() = default;
        virtual ~SDLTexture2D();

        virtual void *GetData() const override
        {
            return reinterpret_cast< void * >( Internal );
        }

        Raster::TextureID Internal = SMR_INVALID_TEXTURE_ID;
        SDL_Surface *pSurface = nullptr;
        uint8_t *pPixels = nullptr;
    };
}
