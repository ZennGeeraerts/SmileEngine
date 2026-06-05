/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/scene/scene_camera.h"
#include "smile/graphic/renderer/resource/frame_buffer.h"

namespace smile::graphic::ecs
{
    struct CameraComponent final
    {
        CameraComponent() = default;
        CameraComponent( const CameraComponent & ) = default;

        SceneCamera Camera;
        bool IsPrimary = true;
        bool HasFixedAspectRatio = false;
        Framebuffer RenderTarget;
    };
}