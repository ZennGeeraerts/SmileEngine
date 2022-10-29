#pragma once
#include <smile_engine.h>

class ExampleLayer final : public Smile::Layer
{
  public:
    ExampleLayer();

    virtual void OnAttach() override;
    virtual void OnUpdate( Smile::Timestep deltaTime ) override;
    virtual void OnEvent( Smile::Event &event ) override;
    virtual void OnImGuiRender() override;

  private:
    bool OnWindowResize( Smile::WindowResizeEvent &e );

  private:
    Smile::Ref< Smile::Scene::Scene > m_pActiveScene;

    Smile::Scene::Entity m_CameraEntity;
    float m_CameraMoveSpeed = 5.f;
    float m_CameraRotationSpeed = 180.f;

    Smile::Scene::Entity m_ModelEntity;

    float m_PrintTimer = 0.f;
};

class MainGame final : public Smile::Application
{
  public:
    MainGame( const Smile::ApplicationDescriptor &descriptor );
    ~MainGame();
};