#pragma once
#include <smile_engine.h>

class ExampleLayer final : public smile::Layer
{
  public:
    ExampleLayer();

    virtual void onAttach() override;
    virtual void onUpdate( smile::Timestep delta_time ) override;
    virtual void onEvent( smile::Event &event ) override;
    virtual void onImGuiRender() override;

  private:
    bool onWindowResize( smile::WindowResizeEvent &e );

  private:
    smile::Ref< smile::scene::Scene > activeScene;

    smile::scene::Entity cameraEntity;
    float cameraMoveSpeed = 5.f;
    float cameraRotationSpeed = 180.f;

    smile::scene::Entity modelEntity;

    float printTimer = 0.f;
};

class MainGame final : public smile::Application
{
  public:
    MainGame( const smile::ApplicationDescriptor &descriptor );
    ~MainGame();
};