#pragma once
#include "smile/graphic/application/graphic_application.h"
#include "world/world.h"
#include "world/entity.h"

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
    smile::Ref< smile::world::World > m_pActiveWorld;

    smile::world::Entity m_CameraEntity;
    float m_CameraMoveSpeed = 5.f;
    float m_CameraRotationSpeed = 180.f;

    smile::world::Entity m_ModelEntity;

    float m_PrintTimer = 0.f;
};

class MainGame final : public smile::graphic::GraphicApplication
{
  public:
    MainGame( const smile::application::ApplicationDescriptor &descriptor );
    ~MainGame();
};