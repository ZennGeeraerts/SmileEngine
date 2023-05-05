#include "main_game.h"

#include <smile_engine/core/entry_point.h>
#include <thirdparty/imgui/imgui.h>

smile::Application *smile::CreateApplication( smile::ApplicationCommandLineArgs commandLineArgs )
{
    smile::ApplicationDescriptor descriptor{};
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
    smile::graphic::RenderCommand::SetClearColor( { DirectX::Colors::DodgerBlue.f[0],
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

    smile::graphic::BufferLayout bufferLayout{ { smile::graphic::ShaderDataType::Float3, "Position" },
        { smile::graphic::ShaderDataType::Float3, "Normal" },
        { smile::graphic::ShaderDataType::Float3, "TexCoord" } };

    smile::Uint32 indices[]{ 0,
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

    m_pActiveScene.reset( new smile::scene::Scene{} );

    m_CameraEntity = m_pActiveScene->CreateEntity( "Camera" );
    auto &camera = m_CameraEntity.AddComponent< smile::scene::CameraComponent >();
    camera.IsPrimary = true;

    /*auto cube = m_pActiveScene->CreateEntity("Cube");
    auto& meshRendererComp = cube.AddComponent<Smile::MeshRendererComponent>();
    meshRendererComp.pVertexBuffer = pVertexBuffer;
    meshRendererComp.pIndexBuffer = pIndexBuffer;
    meshRendererComp.pShader = pShader;

    cube.GetComponent<Smile::TransformComponent>().Translation = DirectX::XMFLOAT3{ -2.5f, 0, 5 };
    cube.GetComponent<Smile::TransformComponent>().Rotation = DirectX::XMFLOAT3{ 45, 45, 0 };*/

    auto pDevice = smile::graphic::RenderEngine::GetDevice();
    auto pShader = pDevice->CreateShader( "assets/shaders/PBR.fx" );
    auto pMaterial = smile::CreateRef< smile::graphic::Material >( pShader );
    smile::Ref< smile::graphic::Texture2D > pAlbedo = pDevice->CreateTexture2D( "assets/textures/uv_grid.png" );
    pMaterial->SetTexture2D( "ALBEDOMAP", pAlbedo );

    m_ModelEntity = m_pActiveScene->CreateEntity( "Model" );
    auto &meshComponent =
        m_ModelEntity.AddComponent< smile::scene::SkinnedMeshComponent >( "assets/meshes/nanosuit.obj", pMaterial );
    m_ModelEntity.GetComponent< smile::scene::TransformComponent >().Translation = DirectX::XMFLOAT3{ 0, -0.1f, 1 };
    m_ModelEntity.GetComponent< smile::scene::TransformComponent >().Rotation = DirectX::XMFLOAT3{ 0.f, 180, 0.f };
    m_ModelEntity.GetComponent< smile::scene::TransformComponent >().Scale = DirectX::XMFLOAT3{ 2, 2, 2 };

    m_pActiveScene->OnViewportResize( 1280, 720 );
    m_pActiveScene->OnRuntimeStart();
}

void ExampleLayer::OnUpdate( smile::Timestep deltaTime )
{
    auto &transform = m_CameraEntity.GetComponent< smile::scene::TransformComponent >();

    if ( smile::input::Input::IsKeyPressed( smile::input::key::Left ) )
        transform.Rotation.y -= DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );
    if ( smile::input::Input::IsKeyPressed( smile::input::key::Right ) )
        transform.Rotation.y += DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );
    if ( smile::input::Input::IsKeyPressed( smile::input::key::Up ) )
        transform.Rotation.x -= DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );
    if ( smile::input::Input::IsKeyPressed( smile::input::key::Down ) )
        transform.Rotation.x += DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );

    const auto forward = transform.GetForward();
    const auto right = transform.GetRight();
    DirectX::XMFLOAT3 move{};

    if ( smile::input::Input::IsKeyPressed( 'A' ) )
        move.x -= 1;
    if ( smile::input::Input::IsKeyPressed( 'D' ) )
        move.x += 1;
    if ( smile::input::Input::IsKeyPressed( 'S' ) )
        move.z -= 1;
    if ( smile::input::Input::IsKeyPressed( 'W' ) )
        move.z += 1;
    if ( smile::input::Input::IsKeyPressed( smile::input::key::Space ) )
        move.y += 1;
    if ( smile::input::Input::IsKeyPressed( smile::input::key::CtrlLeft ) )
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

    m_PrintTimer += smile::Timer::GetInstance().GetDeltaTime();
    if ( m_PrintTimer >= 1.f )
    {
        m_PrintTimer = 0.f;
        smile::Logger::LogInfo( "FPS: %d", smile::Timer::GetInstance().GetFPS() );
    }

    smile::graphic::RenderCommand::Clear();
    m_pActiveScene->OnUpdateRuntime( deltaTime );
}

void ExampleLayer::OnEvent( smile::Event &event )
{
    smile::EventDispatcher dispatcher{ event };
    dispatcher.Dispatch< smile::WindowResizeEvent >( SM_BIND_EVENT_FN( ExampleLayer::OnWindowResize ) );
}

void ExampleLayer::OnImGuiRender()
{
}

bool ExampleLayer::OnWindowResize( smile::WindowResizeEvent &e )
{
    const auto width = e.GetWidth();
    const auto height = e.GetHeight();

    if ( width == 0 || height == 0 )
        return false;

    m_pActiveScene->OnViewportResize( width, height );
    return false;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ Main Game ------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

MainGame::MainGame( const smile::ApplicationDescriptor &descriptor ) : smile::Application{ descriptor }
{
    PushLayer( new ExampleLayer{} );
}

MainGame::~MainGame()
{
}