/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "sdl_texture.h"

namespace smile::graphic
{
    SDLTexture2D::~SDLTexture2D()
    {
        SDL_FreeSurface( pSurface );
    }
}