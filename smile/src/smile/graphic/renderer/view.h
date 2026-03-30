/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        view.h
 * @author      Zenn Geeraerts
 * @created     30 March 2026
 * @brief       Data that is used to render a scene from a specific point of view, such as a camera
 */
#pragma once

#include "smile/graphic/rhi/viewport_state.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    struct ViewConstants final
    {
        DirectX::XMFLOAT4X4 ViewProjectionMatrix;
        DirectX::XMFLOAT4X4 ViewInverseMatrix;
    };

    class View final
    {
      public:
        View() = default;
        ~View() = default;

        void SetViewProjectionMatrix( const DirectX::XMFLOAT4X4 &viewMatrix,
            const DirectX::XMFLOAT4X4 &projectionMatrix );

        void SetViewport( const rhi::Viewport &viewport )
        {
            m_Viewport = viewport;
        }

        void OnUpdate();

        void FillConstants( ViewConstants &constants ) const;

        [[nodiscard]] rhi::ViewportState GetViewportState() const noexcept;

        [[nodiscard]] const DirectX::XMFLOAT4X4 &GetViewMatrix() const noexcept
        {
            return m_ViewMatrix;
        }

        [[nodiscard]] const DirectX::XMFLOAT4X4 &GetViewInverseMatrix() const noexcept
        {
            return m_ViewInverseMatrix;
        }

        [[nodiscard]] const DirectX::XMFLOAT4X4 &GetProjectionMatrix() const noexcept
        {
            return m_ProjectionMatrix;
        }

        [[nodiscard]] const DirectX::XMFLOAT4X4 &GetViewProjectionMatrix() const noexcept
        {
            return m_ViewProjectionMatrix;
        }

      private:
        rhi::Viewport m_Viewport;
        DirectX::XMFLOAT4X4 m_ViewMatrix;
        DirectX::XMFLOAT4X4 m_ViewInverseMatrix;
        DirectX::XMFLOAT4X4 m_ProjectionMatrix;
        DirectX::XMFLOAT4X4 m_ViewProjectionMatrix;

        bool m_IsDirty = true;
    };
}