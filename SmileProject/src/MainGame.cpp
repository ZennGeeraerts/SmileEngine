#include "MainGame.h"
#include "Libs/ImGui/imgui.h"

Smile::SmileGame* Smile::CreateGame()
{
	// This application will get passed to the entry point of the engine
	// and will be deleted once the engine closes
	return new MainGame();
}

/*---------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ Example Layer ------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	
}

void ExampleLayer::OnAttach()
{
	Smile::RenderCommand::SetClearColor({ DirectX::Colors::DodgerBlue.f[0], DirectX::Colors::DodgerBlue.f[1], DirectX::Colors::DodgerBlue.f[2], DirectX::Colors::DodgerBlue.f[3] });
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

	float vertices[]
	{
		-0.5f, 0.5f, -0.5f,		0, 0, 1,
		0.5f, 0.5f, -0.5f,		0, 1, 0,
		-0.5f, -0.5f, -0.5f,	1, 0, 0,
		0.5f, -0.5f, -0.5f,		0, 1, 1,
		-0.5f, 0.5f, 0.5f,		0, 0, 1,
		0.5f, 0.5f, 0.5f,		1, 0, 0,
		-0.5f, -0.5f, 0.5f,		0, 1, 0,
		0.5f, -0.5f, 0.5f,		0, 1, 1
	};

	/*float vertices[]
	{
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f
	};*/

	Smile::BufferLayout bufferLayout
	{
		{ Smile::ShaderDataType::eFloat3, "Position" },
		{ Smile::ShaderDataType::eFloat3, "Color" }
	};

	uint32_t indices[]
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 0, 6,    // side 2
		6, 0, 2,
		7, 5, 6,    // side 3
		6, 5, 4,
		3, 1, 7,    // side 4
		7, 1, 5,
		4, 5, 0,    // side 5
		0, 5, 1,
		3, 7, 2,    // side 6
		2, 7, 6
	};

	Smile::VertexBufferData vertexBufferData{};
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

	Smile::Ref<Smile::Shader> pShader = Smile::Shader::Create("Resources/Shaders/PosCol.fx", bufferLayout);

	m_pActiveScene.reset(new Smile::Scene{});

	/*auto cube = m_pActiveScene->CreateEntity("Cube");
	cube.AddComponent<Smile::MeshRendererComponent>(vertexBufferData, indexBufferData, shaderFilePath);
	auto& cubeTransform = cube.GetComponent<Smile::TransformComponent>();
	cubeTransform.Translation.x -= 10.f;
	cubeTransform.Translation.z += 15.f;*/

	/*m_StormTrooper = m_pActiveScene->CreateEntity("StormTrooper");
	Smile::Ref<Smile::Material> pMaterial = Smile::CreateRef<Smile::Material>();
	m_StormTrooper.AddComponent<Smile::StaticMeshComponent>("Resources/Meshes/silly_dancing.fbx", pMaterial);
	m_StormTrooper.GetComponent<Smile::TransformComponent>().Translation = { 0.f, -5.0f, 15.f };
	m_StormTrooper.GetComponent<Smile::TransformComponent>().Rotation = { 0.f, -180.f, 0.f };
	m_StormTrooper.GetComponent<Smile::TransformComponent>().Scale = { 3, 3, 3 };*/

	m_CameraEntity = m_pActiveScene->CreateEntity("Camera");
	auto& camera = m_CameraEntity.AddComponent<Smile::CameraComponent>();
	camera.bPrimary = true;

	auto cube = m_pActiveScene->CreateEntity("Cube");
	auto& meshRendererComp = cube.AddComponent<Smile::MeshRendererComponent>();
	meshRendererComp.pVertexBuffer = pVertexBuffer;
	meshRendererComp.pIndexBuffer = pIndexBuffer;
	meshRendererComp.pShader = pShader;

	cube.GetComponent<Smile::TransformComponent>().Translation = DirectX::XMFLOAT3{ 0, 0, 5 };
	cube.GetComponent<Smile::TransformComponent>().Rotation = DirectX::XMFLOAT3{ 45, 45, 0 };

	m_pActiveScene->OnViewportResize(1280, 720);
}

void ExampleLayer::OnUpdate(Smile::Timestep deltaTime)
{
	//auto& transform = m_CameraEntity.GetComponent<Smile::TransformComponent>();

	//if (Smile::Input::IsKeyPressed(SM_LEFT))
	//	transform.Rotation.y -= DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);
	//if (Smile::Input::IsKeyPressed(SM_RIGHT))
	//	transform.Rotation.y += DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);
	//if (Smile::Input::IsKeyPressed(SM_UP))
	//	transform.Rotation.x -= DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);
	//if (Smile::Input::IsKeyPressed(SM_DOWN))
	//	transform.Rotation.x += DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);

	//const auto forward = transform.GetForward();
	//const auto right = transform.GetRight();
	//DirectX::XMFLOAT3 move{};

	//if (Smile::Input::IsKeyPressed('A'))
	//	move.x -= 1;
	//if (Smile::Input::IsKeyPressed('D'))
	//	move.x += 1;
	//if (Smile::Input::IsKeyPressed('S'))
	//	move.z -= 1;
	//if (Smile::Input::IsKeyPressed('W'))
	//	move.z += 1;
	//if (Smile::Input::IsKeyPressed(SM_SPACE))
	//	move.y += 1;
	//if (Smile::Input::IsKeyPressed(SM_LCONTROL))
	//	move.y -= 1;

	//DirectX::XMFLOAT3 dir{};
	//dir.x = forward.x * move.z + right.x * move.x;
	////dir.y = forward.y * move.z + right.y * move.x;
	//dir.z = forward.z * move.z + right.z * move.x;

	//auto dirMat = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&dir));
	//DirectX::XMStoreFloat3(&dir, dirMat);

	//transform.Translation.x += dir.x * m_CameraMoveSpeed * deltaTime;
	//transform.Translation.y += dir.y * m_CameraMoveSpeed * deltaTime;
	//transform.Translation.z += dir.z * m_CameraMoveSpeed * deltaTime;

	/*DirectX::XMFLOAT4X4 transform{};
	DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixIdentity());
	Smile::Renderer::Submit(m_pVertexBuffer, m_pIndexBuffer, m_pShader, transform);
	Smile::RenderCommand::Clear();*/
	Smile::RenderCommand::Clear();
	m_pActiveScene->OnUpdate(deltaTime);
}

void ExampleLayer::OnEvent(Smile::Event& event)
{
	Smile::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<Smile::WindowResizeEvent>(SM_BIND_EVENT_FN(ExampleLayer::OnWindowResize));
	dispatcher.Dispatch<Smile::MouseMovedEvent>(SM_BIND_EVENT_FN(ExampleLayer::OnMouseMovedEvent));
}

void ExampleLayer::OnImGuiRender()
{
	
}

bool ExampleLayer::OnWindowResize(Smile::WindowResizeEvent& e)
{
	const auto width = e.GetWidth();
	const auto height = e.GetHeight();

	if (width == 0 || height == 0)
		return false;

	m_pActiveScene->OnViewportResize(width, height);
	return false;
}

bool ExampleLayer::OnMouseMovedEvent(Smile::MouseMovedEvent& e)
{
	/*if (m_bMouseStart)
	{
		m_PreviousMousePosX = e.GetX();
		m_PreviousMousePosY = e.GetY();
		m_bMouseStart = false;
		return false;
	}

	auto& transform = m_CameraEntity.GetComponent<Smile::TransformComponent>();
	Smile::Timestep	deltaTime = Smile::SmTime::GetInstance().GetDeltaTime();

	transform.Rotation.y += DirectX::XMConvertToRadians((e.GetX() - m_PreviousMousePosX) * m_CameraRotationSpeed * deltaTime);
	transform.Rotation.x += DirectX::XMConvertToRadians((e.GetY() - m_PreviousMousePosY) * m_CameraRotationSpeed * deltaTime);

	m_PreviousMousePosX = e.GetX();
	m_PreviousMousePosY = e.GetY();*/

	return false;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ Main Game ------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

MainGame::MainGame()
{
	PushLayer(new ExampleLayer{});
}

MainGame::~MainGame()
{
	
}