/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::graphic
{
    struct BoneInfo final
    {
        Uint32 ID{};
        DirectX::XMFLOAT4X4 Offset{};
    };
}