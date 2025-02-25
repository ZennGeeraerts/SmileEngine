/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "scene_manager.h"

#include "window/window.h"
#include "engine/graphic/renderer/render_command.h"

#include <DirectXColors.h>

namespace smile::graphic
{
    std::vector< Ref< Scene > > SceneManager::s_pScenes{};

    Ref< Scene > SceneManager::CreateScene( const window::Window *pWindow )
    {
        FramebufferDescriptor frameBufferDesc{};
        frameBufferDesc.Attachments = { { FramebufferTextureFormat::RGBA8, true },
            FramebufferTextureFormat::Depth,
            { FramebufferTextureFormat::RGBA8, true } };
        frameBufferDesc.Width = pWindow->GetWidth();
        frameBufferDesc.Height = pWindow->GetHeight();
        frameBufferDesc.IsSwapChainTarget = false;

        Ref< Scene > pScene = CreateRef< Scene >();

        pScene->m_pFramebuffer = RenderCommand::GetGraphicsDevice()->CreateFramebuffer( frameBufferDesc );
        pScene->m_pFramebuffer->ClearColor = { DirectX::Colors::DodgerBlue.f[0],
            DirectX::Colors::DodgerBlue.f[1],
            DirectX::Colors::DodgerBlue.f[2],
            DirectX::Colors::DodgerBlue.f[3] };

        s_pScenes.push_back( pScene );

        return pScene;
    }
}