/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "scene_camera.h"

namespace smile::scene
{
    SceneCamera::SceneCamera()
    {
        RecalculateProjectionMatrix();
    }

    void SceneCamera::SetViewportSize( Uint32 width, Uint32 height )
    {
        m_AspectRatio = width / static_cast< float >( height );

        RecalculateProjectionMatrix();
    }

    void SceneCamera::SetPerspectiveCamera( float fov, float nearPlane, float farPlane )
    {
        m_ProjectionType = ProjectionType::Perspective;
        m_FOV = fov;
        m_PerspectiveNearPlane = nearPlane;
        m_PerspectiveFarPlane = farPlane;

        RecalculateProjectionMatrix();
    }

    void SceneCamera::SetOrthographicCamera( float size, float nearPlane, float farPlane )
    {
        m_ProjectionType = ProjectionType::Orthographic;
        m_Size = size;
        m_OrthographicNearPlane = nearPlane;
        m_OrthographicFarPlane = farPlane;

        RecalculateProjectionMatrix();
    }

    void SceneCamera::RecalculateProjectionMatrix()
    {
        switch ( m_ProjectionType )
        {
            case ProjectionType::Perspective:
            {
                DirectX::XMMATRIX projectionMatrixMat =
                    DirectX::XMMatrixPerspectiveFovLH( DirectX::XMConvertToRadians( m_FOV ),
                        m_AspectRatio,
                        m_PerspectiveNearPlane,
                        m_PerspectiveFarPlane );
                DirectX::XMStoreFloat4x4( &m_ProjectionMatrix, projectionMatrixMat );
                break;
            }
            case ProjectionType::Orthographic:
            {
                float orthoLeft = -m_Size * m_AspectRatio * 0.5f;
                float orthoRight = m_Size * m_AspectRatio * 0.5f;
                float orthoBottom = -m_Size * 0.5f;
                float orthoTop = m_Size * 0.5f;

                DirectX::XMMATRIX projectionMatrixMat = DirectX::XMMatrixOrthographicOffCenterLH(
                    orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNearPlane, m_OrthographicFarPlane );
                DirectX::XMStoreFloat4x4( &m_ProjectionMatrix, projectionMatrixMat );
                break;
            }
        }
    }
}