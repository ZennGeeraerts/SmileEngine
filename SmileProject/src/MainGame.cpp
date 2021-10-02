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
	/*float vertices[]
	{
		0.f, 0.5f, 0.5f,		0.8f, 0.2f, 0.8f,
		0.5f, -0.5f, 0.5f,		0.2f, 0.8f, 0.8f,
		-0.5f, -0.5f, 0.5f,		0.8f, 0.8f, 0.2f
	};*/

	float vertices[]
	{
		-0.5f, 0.5f, -0.5f,		0, 0, 1,
		0.5f, 0.5f, -0.5f,		0, 1, 0,
		-0.5f, -0.5f, -0.5f,		1, 0, 0,
		0.5f, -0.5f, -0.5f,		0, 1, 1,
		-0.5f, 0.5f, 0.5f,		0, 0, 1,
		0.5f, 0.5f, 0.5f,		1, 0, 0,
		-0.5f, -0.5f, 0.5f,		0, 1, 0,
		0.5f, -0.5f, 0.5f,		0, 1, 1
	};

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
	vertexBufferData.BufferLayout = {
		{ Smile::ShaderDataType::eFloat3, "Position" },
		{ Smile::ShaderDataType::eFloat3, "Color" }
	};

	Smile::IndexBufferData indexBufferData{};
	indexBufferData.pIndices = indices;
	indexBufferData.Count = 36;
	indexBufferData.Usage = Smile::BufferUsage::eImmutable;

	m_pActiveScene.reset(new Smile::Scene{});

	/*auto cube = m_pActiveScene->CreateEntity("Cube");
	cube.AddComponent<Smile::MeshRendererComponent>(vertexBufferData, indexBufferData, shaderFilePath);
	auto& cubeTransform = cube.GetComponent<Smile::TransformComponent>();
	cubeTransform.Translation.x -= 10.f;
	cubeTransform.Translation.z += 15.f;*/

	m_GunEntity = m_pActiveScene->CreateEntity("Mesh");
	auto staticMesh = m_GunEntity.AddComponent<Smile::StaticMeshComponent>("../SmileProject/Resources/Meshes/drakefire_pistol_low.obj");
	auto& gunTransform = m_GunEntity.GetComponent<Smile::TransformComponent>();
	gunTransform.Translation.z += 15.f;
	gunTransform.Translation.y -= 2.f;
	gunTransform.Translation.x -= 2.f;
	gunTransform.Rotation.y = 90;
	gunTransform.Scale.x *= 5.f;
	gunTransform.Scale.y *= 5.f;
	gunTransform.Scale.z *= 5.f;

	m_CameraEntity = m_pActiveScene->CreateEntity("Camera");
	m_CameraEntity.AddComponent<Smile::CameraComponent>();
	
	auto pAlbedoMap = Smile::Texture2D::Create("../SmileProject/Resources/Textures/base_albedo.jpg");
	auto pNormalMap = Smile::Texture2D::Create("../SmileProject/Resources/Textures/base_normal.jpg");
	auto pMetalnessMap = Smile::Texture2D::Create("../SmileProject/Resources/Textures/base_metallic.jpg");
	auto pRoughnessMap = Smile::Texture2D::Create("../SmileProject/Resources/Textures/base_roughness.jpg");
	auto pAOMap = Smile::Texture2D::Create("../SmileProject/Resources/Textures/base_AO.jpg");
	auto pEnvironmentMap = Smile::Texture2D::Create("../SmileProject/Resources/Textures/Sunol_Cubemap.dds");
	m_pTexture = Smile::Texture2D::Create("../SmileProject/Resources/Textures/uv_grid.png");

	for (auto& pMesh : staticMesh.pMeshes)
	{
		pMesh->GetShader()->UploadTexture2D("AlbedoMap", pAlbedoMap);
		pMesh->GetShader()->UploadTexture2D("NormalMap", pNormalMap);
		pMesh->GetShader()->UploadTexture2D("MetalnessMap", pMetalnessMap);
		pMesh->GetShader()->UploadTexture2D("RoughnessMap", pRoughnessMap);
		pMesh->GetShader()->UploadTexture2D("AOMap", pAOMap);
		pMesh->GetShader()->UploadTexture2D("EnvironmentMap", pEnvironmentMap);
	}

	Smile::FramebufferData framebufferData{};
	framebufferData.Width = 1280;
	framebufferData.Height = 720;
}

void ExampleLayer::OnUpdate(Smile::Timestep deltaTime)
{
	auto& transform = m_CameraEntity.GetComponent<Smile::TransformComponent>();

	if (Smile::Input::IsKeyPressed(SM_LEFT))
		transform.Rotation.y -= DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);
	if (Smile::Input::IsKeyPressed(SM_RIGHT))
		transform.Rotation.y += DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);
	if (Smile::Input::IsKeyPressed(SM_UP))
		transform.Rotation.x -= DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);
	if (Smile::Input::IsKeyPressed(SM_DOWN))
		transform.Rotation.x += DirectX::XMConvertToRadians(m_CameraRotationSpeed * deltaTime);

	const auto forward = transform.GetForward();
	const auto right = transform.GetRight();
	DirectX::XMFLOAT3 move{};

	if (Smile::Input::IsKeyPressed('A'))
		move.x -= 1;
	if (Smile::Input::IsKeyPressed('D'))
		move.x += 1;
	if (Smile::Input::IsKeyPressed('S'))
		move.z -= 1;
	if (Smile::Input::IsKeyPressed('W'))
		move.z += 1;
	if (Smile::Input::IsKeyPressed(SM_SPACE))
		move.y += 1;
	if (Smile::Input::IsKeyPressed(SM_LCONTROL))
		move.y -= 1;

	DirectX::XMFLOAT3 dir{};
	dir.x = forward.x * move.z + right.x * move.x;
	//dir.y = forward.y * move.z + right.y * move.x;
	dir.z = forward.z * move.z + right.z * move.x;

	auto dirMat = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&dir));
	DirectX::XMStoreFloat3(&dir, dirMat);

	transform.Translation.x += dir.x * m_CameraMoveSpeed * deltaTime;
	transform.Translation.y += dir.y * m_CameraMoveSpeed * deltaTime;
	transform.Translation.z += dir.z * m_CameraMoveSpeed * deltaTime;

	auto& gunTransform = m_GunEntity.GetComponent<Smile::TransformComponent>();
	gunTransform.Rotation.y += 1.f * deltaTime;

	Smile::RenderCommand::SetClearColor({ DirectX::Colors::DodgerBlue.f[0], DirectX::Colors::DodgerBlue.f[1], DirectX::Colors::DodgerBlue.f[2], DirectX::Colors::DodgerBlue.f[3] });
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

	auto& cameraComponent = m_CameraEntity.GetComponent<Smile::CameraComponent>();
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