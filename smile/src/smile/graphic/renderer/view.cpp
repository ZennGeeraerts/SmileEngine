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
 * @file        view.cpp
 * @author      Zenn Geeraerts
 * @created     30 March 2026
 * @brief       Data that is used to render a scene from a specific point of view, such as a camera
 */
#include "smpch.h"
#include "view.h"

namespace smile::graphic
{
    void View::SetViewProjectionMatrix( const DirectX::XMFLOAT4X4 &viewMatrix,
        const DirectX::XMFLOAT4X4 &projectionMatrix )
    {
        m_ViewMatrix = viewMatrix;
        m_ProjectionMatrix = projectionMatrix;

        m_IsDirty = true;
    }

    void View::OnUpdate()
    {
        if ( !m_IsDirty )
            return;

        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &m_ProjectionMatrix );
        auto viewMatrixMat = DirectX::XMLoadFloat4x4( &m_ViewMatrix );
        auto viewProjectionMatrixMat = viewMatrixMat * projectionMatrixMat;

        DirectX::XMStoreFloat4x4( &m_ViewProjectionMatrix, viewProjectionMatrixMat );
        DirectX::XMStoreFloat4x4( &m_ViewInverseMatrix, DirectX::XMMatrixInverse( nullptr, viewMatrixMat ) );

        m_IsDirty = false;
    }

    void View::FillConstants( ViewConstants &constants ) const
    {
        constants.ViewProjectionMatrix = m_ViewProjectionMatrix;
        constants.ViewInverseMatrix = m_ViewInverseMatrix;
    }

    rhi::ViewportState View::GetViewportState() const noexcept
    {
        rhi::ViewportState viewportState{};
        viewportState.Viewports.PushBack( m_Viewport );

        return viewportState;
    }
}