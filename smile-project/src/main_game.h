#pragma once
#include <smile_engine.h>

class ExampleLayer final : public smile::application::Layer
{
  public:
    ExampleLayer();

    virtual void OnAttach() override;
    virtual void OnUpdate( smile::primitive::Timestep deltaTime ) override;
    virtual void OnEvent( smile::window::Event &event ) override;
    virtual void OnImGuiRender() override;

  private:
    bool OnWindowResize( smile::window::WindowResizeEvent &e );

  private:
    smile::Ref< smile::scene::Scene > m_pActiveScene;

    smile::scene::Entity m_CameraEntity;
    float m_CameraMoveSpeed = 5.f;
    float m_CameraRotationSpeed = 180.f;

    smile::scene::Entity m_ModelEntity;

    float m_PrintTimer = 0.f;
};

class MainGame final : public smile::application::Application
{
  public:
    MainGame( const smile::application::ApplicationDescriptor &descriptor );
    ~MainGame();
};