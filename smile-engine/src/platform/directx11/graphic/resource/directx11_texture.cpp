/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_texture.h"

namespace smile::graphic
{
    DirectX11Texture2D::~DirectX11Texture2D()
    {
        SAFE_RELEASE( pTexture );
        SAFE_RELEASE( pShaderResourceView );
    }

    DirectX11TextureCube::~DirectX11TextureCube()
    {
        SAFE_RELEASE( pTexture );
        SAFE_RELEASE( pShaderResourceView );
    }
}