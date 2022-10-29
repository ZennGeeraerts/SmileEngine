#include "main_game.h"

#include <smile_engine/core/entry_point.h>
#include <thirdparty/imgui/imgui.h>

Smile::Application *Smile::CreateApplication( Smile::ApplicationCommandLineArgs commandLineArgs )
{
    Smile::ApplicationDescriptor descriptor{};
    descriptor.Name = "Main Game";
    descriptor.WorkingDirectory = "../smile-editor";
    descriptor.CommandLineArgs = commandLineArgs;

    // This application will get passed to the entry point of the engine
    // and will be deleted once the engine closes
    return new MainGame{ descriptor };
}

/*---------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ Example Layer ------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

ExampleLayer::ExampleLayer() : Layer( "Example" )
{
}

void ExampleLayer::OnAttach()
{
    Smile::Graphic::RenderCommand::SetClearColor( { DirectX::Colors::DodgerBlue.f[0],
        DirectX::Colors::DodgerBlue.f[1],
        DirectX::Colors::DodgerBlue.f[2],
        DirectX::Colors::DodgerBlue.f[3] } );
    /*float vertices[]
    {
        0.f, 0.5f, 0.5f,		0.8f, 0.2f, 0.8f,
        0.5f, -0.5f, 0.5f,		0.2f, 0.8f, 0.8f,
        -0.5f, -0.5f, 0.5f,		0.8f, 0.8f, 0.2f
    };*/

    /*float vertices[]
    {
        0.f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f
    };*/

    /*uint32_t indices[]
    {
        0, 1, 2
    };*/

    /*float vertices[]
    {
        -0.5f, 0.5f, -0.5f,		0, 0, 1,
        0.5f, 0.5f, -0.5f,		0, 1, 0,
        -0.5f, -0.5f, -0.5f,	1, 0, 0,
        0.5f, -0.5f, -0.5f,		0, 1, 1,
        -0.5f, 0.5f, 0.5f,		0, 0, 1,
        0.5f, 0.5f, 0.5f,		1, 0, 0,
        -0.5f, -0.5f, 0.5f,		0, 1, 0,
        0.5f, -0.5f, 0.5f,		0, 1, 1
    };*/

    float vertices[]{ -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f };

    Smile::Graphic::BufferLayout bufferLayout{ { Smile::Graphic::ShaderDataType::Float3, "Position" },
        { Smile::Graphic::ShaderDataType::Float3, "Normal" },
        { Smile::Graphic::ShaderDataType::Float3, "TexCoord" } };

    Smile::Uint32 indices[]{ 0,
        1,
        2, // side 1
        2,
        1,
        3,
        4,
        0,
        6, // side 2
        6,
        0,
        2,
        7,
        5,
        6, // side 3
        6,
        5,
        4,
        3,
        1,
        7, // side 4
        7,
        1,
        5,
        4,
        5,
        0, // side 5
        0,
        5,
        1,
        3,
        7,
        2, // side 6
        2,
        7,
        6 };

    /*Smile::VertexBufferData vertexBufferData{};
    vertexBufferData.pVertices = vertices;
    vertexBufferData.Count = 8;
    vertexBufferData.Usage = Smile::BufferUsage::eImmutable;
    vertexBufferData.BufferLayout = bufferLayout;

    Smile::Ref<Smile::VertexBuffer> pVertexBuffer{};
    pVertexBuffer.reset(Smile::VertexBuffer::Create(vertexBufferData));

    Smile::IndexBufferData indexBufferData{};
    indexBufferData.pIndices = indices;
    indexBufferData.Count = 36;
    indexBufferData.Usage = Smile::BufferUsage::eImmutable;

    Smile::Ref<Smile::IndexBuffer> pIndexBuffer{};
    pIndexBuffer.reset(Smile::IndexBuffer::Create(indexBufferData));

    Smile::Ref<Smile::Shader> pShader = Smile::Shader::Create("Resources/Shaders/PosNormTex.fx", bufferLayout);*/

    m_pActiveScene.reset( new Smile::Scene::Scene{} );

    m_CameraEntity = m_pActiveScene->CreateEntity( "Camera" );
    auto &camera = m_CameraEntity.AddComponent< Smile::Scene::CameraComponent >();
    camera.IsPrimary = true;

    /*auto cube = m_pActiveScene->CreateEntity("Cube");
    auto& meshRendererComp = cube.AddComponent<Smile::MeshRendererComponent>();
    meshRendererComp.pVertexBuffer = pVertexBuffer;
    meshRendererComp.pIndexBuffer = pIndexBuffer;
    meshRendererComp.pShader = pShader;

    cube.GetComponent<Smile::TransformComponent>().Translation = DirectX::XMFLOAT3{ -2.5f, 0, 5 };
    cube.GetComponent<Smile::TransformComponent>().Rotation = DirectX::XMFLOAT3{ 45, 45, 0 };*/

    auto pShader = Smile::Graphic::Shader::Create( "assets/shaders/PBR.fx" );
    auto pMaterial = Smile::CreateRef< Smile::Graphic::Material >( pShader );
    Smile::Ref< Smile::Graphic::Texture2D > pAlbedo =
        Smile::Graphic::Texture2D::Create( "assets/textures/uv_grid.png" );
    pMaterial->SetTexture2D( "ALBEDOMAP", pAlbedo );

    m_ModelEntity = m_pActiveScene->CreateEntity( "Model" );
    auto &meshComponent =
        m_ModelEntity.AddComponent< Smile::Scene::SkinnedMeshComponent >( "assets/meshes/nanosuit.obj", pMaterial );
    m_ModelEntity.GetComponent< Smile::Scene::TransformComponent >().Translation = DirectX::XMFLOAT3{ 0, -0.1f, 1 };
    m_ModelEntity.GetComponent< Smile::Scene::TransformComponent >().Rotation = DirectX::XMFLOAT3{ 0.f, 180, 0.f };
    m_ModelEntity.GetComponent< Smile::Scene::TransformComponent >().Scale = DirectX::XMFLOAT3{ 2, 2, 2 };

    m_pActiveScene->OnViewportResize( 1280, 720 );
    m_pActiveScene->OnRuntimeStart();
}

void ExampleLayer::OnUpdate( Smile::Timestep deltaTime )
{
    auto &transform = m_CameraEntity.GetComponent< Smile::Scene::TransformComponent >();

    if ( Smile::Input::Input::IsKeyPressed( Smile::Input::key::Left ) )
        transform.Rotation.y -= DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );
    if ( Smile::Input::Input::IsKeyPressed( Smile::Input::key::Right ) )
        transform.Rotation.y += DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );
    if ( Smile::Input::Input::IsKeyPressed( Smile::Input::key::Up ) )
        transform.Rotation.x -= DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );
    if ( Smile::Input::Input::IsKeyPressed( Smile::Input::key::Down ) )
        transform.Rotation.x += DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );

    const auto forward = transform.getForward();
    const auto right = transform.getRight();
    DirectX::XMFLOAT3 move{};

    if ( Smile::Input::Input::IsKeyPressed( 'A' ) )
        move.x -= 1;
    if ( Smile::Input::Input::IsKeyPressed( 'D' ) )
        move.x += 1;
    if ( Smile::Input::Input::IsKeyPressed( 'S' ) )
        move.z -= 1;
    if ( Smile::Input::Input::IsKeyPressed( 'W' ) )
        move.z += 1;
    if ( Smile::Input::Input::IsKeyPressed( Smile::Input::key::Space ) )
        move.y += 1;
    if ( Smile::Input::Input::IsKeyPressed( Smile::Input::key::CtrlLeft ) )
        move.y -= 1;

    DirectX::XMFLOAT3 dir{};
    dir.x = forward.x * move.z + right.x * move.x;
    // dir.y = forward.y * move.z + right.y * move.x;
    dir.z = forward.z * move.z + right.z * move.x;

    auto dirVec = DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &dir ) );
    DirectX::XMStoreFloat3( &dir, dirVec );

    transform.Translation.x += dir.x * m_CameraMoveSpeed * deltaTime;
    transform.Translation.y += dir.y * m_CameraMoveSpeed * deltaTime;
    transform.Translation.z += dir.z * m_CameraMoveSpeed * deltaTime;

    // m_ModelEntity.GetComponent<Smile::TransformComponent>().Rotation.y += 1.f * deltaTime;

    m_PrintTimer += Smile::Timer::GetInstance().GetDeltaTime();
    if ( m_PrintTimer >= 1.f )
    {
        m_PrintTimer = 0.f;
        Smile::Logger::LogInfo( "FPS: %d", Smile::Timer::GetInstance().GetFPS() );
    }

    Smile::Graphic::RenderCommand::Clear();
    m_pActiveScene->OnUpdateRuntime( deltaTime );
}

void ExampleLayer::OnEvent( Smile::Event &event )
{
    Smile::EventDispatcher dispatcher{ event };
    dispatcher.Dispatch< Smile::WindowResizeEvent >( SM_BIND_EVENT_FN( ExampleLayer::OnWindowResize ) );
}

void ExampleLayer::OnImGuiRender()
{
}

bool ExampleLayer::OnWindowResize( Smile::WindowResizeEvent &e )
{
    const auto width = e.getWidth();
    const auto height = e.getHeight();

    if ( width == 0 || height == 0 )
        return false;

    m_pActiveScene->OnViewportResize( width, height );
    return false;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ Main Game ------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

MainGame::MainGame( const Smile::ApplicationDescriptor &descriptor ) : Smile::Application{ descriptor }
{
    PushLayer( new ExampleLayer{} );
}

MainGame::~MainGame()
{
}