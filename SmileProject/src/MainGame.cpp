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

	m_Gun = m_pActiveScene->CreateEntity("Mesh");
	auto staticMesh = m_Gun.AddComponent<Smile::StaticMeshComponent>("../SmileProject/Resources/Meshes/drakefire_pistol_low.obj");
	auto& gunTransform = m_Gun.GetComponent<Smile::TransformComponent>();
	gunTransform.Translation.z += 15.f;
	gunTransform.Translation.y -= 2.f;
	gunTransform.Translation.x -= 2.f;
	gunTransform.Rotation.y = 90;
	gunTransform.Scale.x *= 5.f;
	gunTransform.Scale.y *= 5.f;
	gunTransform.Scale.z *= 5.f;

	m_CameraEntity = m_pActiveScene->CreateEntity("Camera");
	m_CameraEntity.AddComponent<Smile::CameraComponent>(DirectX::XMMatrixPerspectiveFovLH(45, 16 / 9.f, 0.1f, 2500.f));
	
	auto pAlbedoMap = Smile::Texture2D::Create("../SmileProject/Resources/Textures/base_albedo.jpg");
	auto pNormalMap = Smile::Texture2D::Create("../SmileProject/Resources/Textures/base_normal.jpg");
	auto pMetalnessMap = Smile::Texture2D::Create("../SmileProject/Resources/Textures/base_metallic.jpg");
	auto pRoughnessMap = Smile::Texture2D::Create("../SmileProject/Resources/Textures/base_roughness.jpg");
	auto pAOMap = Smile::Texture2D::Create("../SmileProject/Resources/Textures/base_AO.jpg");

	for (auto& pMesh : staticMesh.m_pMeshes)
	{
		pMesh->GetShader()->UploadTexture2D("AlbedoMap", pAlbedoMap);
		pMesh->GetShader()->UploadTexture2D("NormalMap", pNormalMap);
		pMesh->GetShader()->UploadTexture2D("MetalnessMap", pMetalnessMap);
		pMesh->GetShader()->UploadTexture2D("RoughnessMap", pRoughnessMap);
		pMesh->GetShader()->UploadTexture2D("AOMap", pAOMap);
	}
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
	DirectX::XMFLOAT3 dir{};

	if (Smile::Input::IsKeyPressed('A'))
		dir.x -= 1;
	if (Smile::Input::IsKeyPressed('D'))
		dir.x += 1;
	if (Smile::Input::IsKeyPressed('S'))
		dir.z -= 1;
	if (Smile::Input::IsKeyPressed('W'))
		dir.z += 1;
	if (Smile::Input::IsKeyPressed(SM_SPACE))
		dir.y += 1;
	if (Smile::Input::IsKeyPressed(SM_LCONTROL))
		dir.y -= 1;

	dir.x = forward.x * dir.z + right.x * dir.x;
	//dir.y = forward.y * dir.z + right.y * dir.x;
	dir.z = forward.z * dir.z + right.z * dir.x;

	auto dirMat = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&dir));
	DirectX::XMStoreFloat3(&dir, dirMat);

	transform.Translation.x += dir.x * m_CameraMoveSpeed * deltaTime;
	transform.Translation.y += dir.y * m_CameraMoveSpeed * deltaTime;
	transform.Translation.z += dir.z * m_CameraMoveSpeed * deltaTime;

	auto& gunTransform = m_Gun.GetComponent<Smile::TransformComponent>();
	gunTransform.Rotation.y += 1.f * deltaTime;

	Smile::RenderCommand::SetClearColor({ DirectX::Colors::DodgerBlue.f[0], DirectX::Colors::DodgerBlue.f[1], DirectX::Colors::DodgerBlue.f[2], DirectX::Colors::DodgerBlue.f[3] });
	Smile::RenderCommand::Clear();

	m_pActiveScene->OnUpdate(deltaTime);
}

void ExampleLayer::OnEvent(Smile::Event& event)
{
	Smile::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<Smile::WindowResizeEvent>(SM_BIND_EVENT_FN(ExampleLayer::OnWindowResize));
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Triangle Color", &m_CameraPosition.x);
	ImGui::End();
}

bool ExampleLayer::OnWindowResize(Smile::WindowResizeEvent& e)
{
	const auto width = e.GetWidth();
	const auto height = e.GetHeight();

	if (width == 0 || height == 0)
		return false;

	auto& cameraComponent = m_CameraEntity.GetComponent<Smile::CameraComponent>();
	cameraComponent = DirectX::XMMatrixPerspectiveFovLH(45, width / static_cast<float>(height), 0.1f, 2500.f);
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