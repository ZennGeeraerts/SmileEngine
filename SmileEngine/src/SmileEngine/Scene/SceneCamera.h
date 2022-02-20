#pragma once
#include "SmileEngine/Renderer/Camera/Camera.h"

namespace smile
{
    class SceneCamera final : public Camera
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

        void SetViewportSize( uint32_t width, uint32_t height );

        ProjectionType GetProjectionType() const
        {
            return m_ProjectionType;
        }
        void SetProjectionType( ProjectionType projectionType )
        {
            m_ProjectionType = projectionType;
            RecalculateProjectionMatrix();
        }

        // Perspective
        void SetPerspectiveCamera( float fov, float nearPlane, float farPlane );

        float GetFOV() const
        {
            return m_FOV;
        }
        void SetFOV( float fov )
        {
            m_FOV = fov;
            RecalculateProjectionMatrix();
        }

        float GetPerspectiveNearPlane() const
        {
            return m_PerspectiveNearPlane;
        }
        void SetPerspectiveNearPlane( float nearPlane )
        {
            m_PerspectiveNearPlane = nearPlane;
            RecalculateProjectionMatrix();
        }

        float GetPerspectiveFarPlane() const
        {
            return m_PerspectiveFarPlane;
        }
        void SetPerspectiveFarPlane( float farPlane )
        {
            m_PerspectiveFarPlane = farPlane;
            RecalculateProjectionMatrix();
        }

        // Orthographic
        void SetOrthographicCamera( float size, float nearPlane, float farPlane );

        float GetSize() const
        {
            return m_Size;
        }
        void SetSize( float size )
        {
            m_Size = size;
            RecalculateProjectionMatrix();
        }

        float GetOrthographicNearPlane() const
        {
            return m_OrthographicNearPlane;
        }
        void SetOrthographicNearPlane( float nearPlane )
        {
            m_OrthographicNearPlane = nearPlane;
            RecalculateProjectionMatrix();
        }

        float GetOrthographicFarPlane() const
        {
            return m_OrthographicFarPlane;
        }
        void SetOrthographicFarPlane( float farPlane )
        {
            m_OrthographicFarPlane = farPlane;
            RecalculateProjectionMatrix();
        }

      private:
        void RecalculateProjectionMatrix();

      private:
        ProjectionType m_ProjectionType = ProjectionType::Perspective;

        // Perspective
        float m_FOV = 45.f;
        float m_PerspectiveNearPlane = 0.1f;
        float m_PerspectiveFarPlane = 2500.f;

        // Orthographic
        float m_Size = 10.f;
        float m_OrthographicNearPlane = -1.f;
        float m_OrthographicFarPlane = 1.f;

        float m_AspectRatio = 1.0f;
    };
}
