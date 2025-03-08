/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_texture.h"

namespace smile::graphic
{
    DirectX11Texture::~DirectX11Texture()
    {
        SAFE_RELEASE( pTexture );
        SAFE_RELEASE( pShaderResourceView );
    }
}