#include "main_game.h"

#include <smile_engine/core/entry_point.h>
#include <thirdparty/imgui/imgui.h>

smile::Application *smile::createGame()
{
    // This application will get passed to the entry point of the engine
    // and will be deleted once the engine closes
    return new MainGame();
}

/*---------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ Example Layer ------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

ExampleLayer::ExampleLayer() : Layer( "Example" )
{
}

void ExampleLayer::onAttach()
{
    smile::renderer::RenderCommand::setClearColor( { DirectX::Colors::DodgerBlue.f[0],
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

    smile::renderer::BufferLayout buffer_layout{ { smile::renderer::ShaderDataType::Float3, "Position" },
        { smile::renderer::ShaderDataType::Float3, "Normal" },
        { smile::renderer::ShaderDataType::Float3, "TexCoord" } };

    uint32_t indices[]{ 0,
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

    activeScene.reset( new smile::scene::Scene{} );

    cameraEntity = activeScene->createEntity( "Camera" );
    auto &camera = cameraEntity.addComponent< smile::scene::CameraComponent >();
    camera.primary = true;

    /*auto cube = m_pActiveScene->CreateEntity("Cube");
    auto& meshRendererComp = cube.AddComponent<Smile::MeshRendererComponent>();
    meshRendererComp.pVertexBuffer = pVertexBuffer;
    meshRendererComp.pIndexBuffer = pIndexBuffer;
    meshRendererComp.pShader = pShader;

    cube.GetComponent<Smile::TransformComponent>().Translation = DirectX::XMFLOAT3{ -2.5f, 0, 5 };
    cube.GetComponent<Smile::TransformComponent>().Rotation = DirectX::XMFLOAT3{ 45, 45, 0 };*/

    auto shader = smile::renderer::Shader::create( "assets/shaders/PBR.fx" );
    auto material = smile::createRef< smile::renderer::Material >( shader );
    smile::Ref< smile::renderer::Texture2D > pAlbedo = smile::renderer::Texture2D::create( "Resources/Textures/uv_grid.png" );
    material->setTexture2D( "ALBEDO", pAlbedo );

    modelEntity = activeScene->createEntity( "Model" );
    auto &mesh_component =
        modelEntity.addComponent< smile::scene::SkinnedMeshComponent >( "Resources/Meshes/bunny.obj", material );
    modelEntity.getComponent< smile::scene::TransformComponent >().translation = DirectX::XMFLOAT3{ 0, -0.1f, 1 };
    modelEntity.getComponent< smile::scene::TransformComponent >().rotation = DirectX::XMFLOAT3{ 0.f, 180, 0.f };
    modelEntity.getComponent< smile::scene::TransformComponent >().scale = DirectX::XMFLOAT3{ 2, 2, 2 };

    activeScene->onViewportResize( 1280, 720 );
}

void ExampleLayer::onUpdate( smile::Timestep delta_time )
{
    auto &transform = cameraEntity.getComponent< smile::scene::TransformComponent >();

    if ( smile::input::Input::isKeyPressed( smile::input::key::Left ) )
        transform.rotation.y -= DirectX::XMConvertToRadians( cameraRotationSpeed * delta_time );
    if ( smile::input::Input::isKeyPressed( smile::input::key::Right ) )
        transform.rotation.y += DirectX::XMConvertToRadians( cameraRotationSpeed * delta_time );
    if ( smile::input::Input::isKeyPressed( smile::input::key::Up ) )
        transform.rotation.x -= DirectX::XMConvertToRadians( cameraRotationSpeed * delta_time );
    if ( smile::input::Input::isKeyPressed( smile::input::key::Down ) )
        transform.rotation.x += DirectX::XMConvertToRadians( cameraRotationSpeed * delta_time );

    const auto forward = transform.getForward();
    const auto right = transform.getRight();
    DirectX::XMFLOAT3 move{};

    if ( smile::input::Input::isKeyPressed( 'A' ) )
        move.x -= 1;
    if ( smile::input::Input::isKeyPressed( 'D' ) )
        move.x += 1;
    if ( smile::input::Input::isKeyPressed( 'S' ) )
        move.z -= 1;
    if ( smile::input::Input::isKeyPressed( 'W' ) )
        move.z += 1;
    if ( smile::input::Input::isKeyPressed( smile::input::key::Space ) )
        move.y += 1;
    if ( smile::input::Input::isKeyPressed( smile::input::key::CtrlLeft ) )
        move.y -= 1;

    DirectX::XMFLOAT3 dir{};
    dir.x = forward.x * move.z + right.x * move.x;
    // dir.y = forward.y * move.z + right.y * move.x;
    dir.z = forward.z * move.z + right.z * move.x;

    auto dir_vec = DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &dir ) );
    DirectX::XMStoreFloat3( &dir, dir_vec );

    transform.translation.x += dir.x * cameraMoveSpeed * delta_time;
    transform.translation.y += dir.y * cameraMoveSpeed * delta_time;
    transform.translation.z += dir.z * cameraMoveSpeed * delta_time;

    // m_ModelEntity.GetComponent<Smile::TransformComponent>().Rotation.y += 1.f * deltaTime;

    printTimer += smile::Timer::getInstance().getDeltaTime();
    if ( printTimer >= 1.f )
    {
        printTimer = 0.f;
        smile::Logger::logInfo( "FPS: %d", smile::Timer::getInstance().getFPS() );
    }

    smile::renderer::RenderCommand::clear();
    activeScene->onUpdateRuntime( delta_time );
}

void ExampleLayer::onEvent( smile::Event &event )
{
    smile::EventDispatcher dispatcher{ event };
    dispatcher.dispatch< smile::WindowResizeEvent >( SM_BIND_EVENT_FN( ExampleLayer::onWindowResize ) );
}

void ExampleLayer::onImGuiRender()
{
}

bool ExampleLayer::onWindowResize( smile::WindowResizeEvent &e )
{
    const auto width = e.getWidth();
    const auto height = e.getHeight();

    if ( width == 0 || height == 0 )
        return false;

    activeScene->onViewportResize( width, height );
    return false;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ Main Game ------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

MainGame::MainGame()
{
    pushLayer( new ExampleLayer{} );
}

MainGame::~MainGame()
{
}