#pragma once
#include "smile/graphic/application/graphic_application.h"
#include "smile/core/world/world.h"
#include "smile/core/world/entity.h"

class ExampleLayer final : public smile::application::Layer
{
  public:
    ExampleLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate( smile::primitive::Timestep deltaTime ) override;
    void OnEvent( smile::window::Event &event ) override;
    void OnImGuiRender() override;

  private:
    bool OnWindowResize( smile::window::WindowResizeEvent &e );

  private:
    smile::memory::Ref< smile::world::World > m_pActiveWorld;

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