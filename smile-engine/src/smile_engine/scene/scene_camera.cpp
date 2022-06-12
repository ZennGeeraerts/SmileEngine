#include "smpch.h"
#include "scene_camera.h"

namespace smile::scene
{
    SceneCamera::SceneCamera()
    {
        recalculateProjectionMatrix();
    }

    void SceneCamera::setViewportSize( Uint32 width, Uint32 height )
    {
        aspectRatio = width / static_cast< float >( height );

        recalculateProjectionMatrix();
    }

    void SceneCamera::setPerspectiveCamera( float new_fov, float near_plane, float far_plane )
    {
        projectionType = ProjectionType::Perspective;
        fov = new_fov;
        perspectiveNearPlane = near_plane;
        perspectiveFarPlane = far_plane;

        recalculateProjectionMatrix();
    }

    void SceneCamera::setOrthographicCamera( float new_size, float near_plane, float far_plane )
    {
        projectionType = ProjectionType::Orthographic;
        size = new_size;
        perspectiveNearPlane = near_plane;
        perspectiveFarPlane = far_plane;

        recalculateProjectionMatrix();
    }

    void SceneCamera::recalculateProjectionMatrix()
    {
        switch ( projectionType )
        {
            case ProjectionType::Perspective:
            {
                DirectX::XMMATRIX projection_matrix_mat =
                    DirectX::XMMatrixPerspectiveFovLH( DirectX::XMConvertToRadians( fov ),
                        aspectRatio,
                        perspectiveNearPlane,
                        perspectiveFarPlane );
                DirectX::XMStoreFloat4x4( &projectionMatrix, projection_matrix_mat );
                break;
            }
            case ProjectionType::Orthographic:
            {
                float ortho_left = -size * aspectRatio * 0.5f;
                float ortho_right = size * aspectRatio * 0.5f;
                float ortho_bottom = -size * 0.5f;
                float ortho_top = size * 0.5f;

                DirectX::XMMATRIX projection_matrix_mat = DirectX::XMMatrixOrthographicOffCenterLH(
                    ortho_left, ortho_right, ortho_bottom, ortho_top, orthographicNearPlane, orthographicFarPlane );
                DirectX::XMStoreFloat4x4( &projectionMatrix, projection_matrix_mat );
                break;
            }
        }
    }
}