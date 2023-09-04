/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "model_loader_utils.h"

namespace smile::graphic::utils
{
    DirectX::XMFLOAT3 ConvertToVector3( const aiVector3D &aiVector )
    {
        return *( DirectX::XMFLOAT3 * )( &aiVector );
    }

    DirectX::XMFLOAT4 ConvertToVector4( const aiColor4D &aiColor )
    {
        return *( DirectX::XMFLOAT4 * )( &aiColor );
    }
}