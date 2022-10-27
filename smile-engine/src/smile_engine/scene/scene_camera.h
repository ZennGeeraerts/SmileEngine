#pragma once
#include "smile_engine/graphic/camera/camera.h"

namespace smile::scene
{
    class SceneCamera final : public graphic::Camera
    {
      public:
        enum class ProjectionType
        {
            Perspective = 0,
            Orthographic = 1
        };

      public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void setViewportSize( Uint32 width, Uint32 height );

        ProjectionType getProjectionType() const
        {
            return projectionType;
        }
        void setProjectionType( ProjectionType projection_type )
        {
            projectionType = projection_type;
            recalculateProjectionMatrix();
        }

        // Perspective
        void setPerspectiveCamera( float new_fov, float near_plane, float far_plane );

        float getFOV() const
        {
            return fov;
        }
        void setFOV( float new_fov )
        {
            fov = new_fov;
            recalculateProjectionMatrix();
        }

        float getPerspectiveNearPlane() const
        {
            return perspectiveNearPlane;
        }
        void setPerspectiveNearPlane( float near_plane )
        {
            perspectiveNearPlane = near_plane;
            recalculateProjectionMatrix();
        }

        float getPerspectiveFarPlane() const
        {
            return perspectiveFarPlane;
        }
        void setPerspectiveFarPlane( float far_plane )
        {
            perspectiveFarPlane = far_plane;
            recalculateProjectionMatrix();
        }

        // Orthographic
        void setOrthographicCamera( float new_size, float near_plane, float far_plane );

        float getSize() const
        {
            return size;
        }
        void setSize( float new_size )
        {
            size = new_size;
            recalculateProjectionMatrix();
        }

        float getOrthographicNearPlane() const
        {
            return orthographicNearPlane;
        }
        void setOrthographicNearPlane( float near_plane )
        {
            orthographicNearPlane = near_plane;
            recalculateProjectionMatrix();
        }

        float getOrthographicFarPlane() const
        {
            return orthographicFarPlane;
        }
        void setOrthographicFarPlane( float far_plane )
        {
            orthographicFarPlane = far_plane;
            recalculateProjectionMatrix();
        }

      private:
        void recalculateProjectionMatrix();

      private:
        ProjectionType projectionType = ProjectionType::Perspective;

        // Perspective
        float fov = 45.f;
        float perspectiveNearPlane = 0.1f;
        float perspectiveFarPlane = 2500.f;

        // Orthographic
        float size = 10.f;
        float orthographicNearPlane = -1.f;
        float orthographicFarPlane = 1.f;

        float aspectRatio = 1.0f;
    };
}
