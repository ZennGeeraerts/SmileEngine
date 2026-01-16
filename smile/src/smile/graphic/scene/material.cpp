/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "material.h"

#include "smile/graphic/renderer/render_engine.h"

namespace smile::graphic
{
    Material::Material( const VertexShader::ConstRef &pVertexShader, const PixelShader::ConstRef &pPixelShader ) noexcept
    {
        SetShaders( pVertexShader, pPixelShader );
    }

    Material::~Material()
    {
    }
}