#include "MainGame.h"
#include "SmileEngine/ImGui/ImGui/imgui.h"

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
	, m_Camera{ -1.6f, 1.6f, -0.9f, 0.9f }
	, m_CameraPosition{ 0.f, 0.f, 0.f }
{
	/*float vertices[]
	{
		0.f, 0.5f, 0.5f,		0.8f, 0.2f, 0.8f,
		0.5f, -0.5f, 0.5f,		0.2f, 0.8f, 0.8f,
		-0.5f, -0.5f, 0.5f,		0.8f, 0.8f, 0.2f
	};*/

	float vertices[]
	{
		-0.5f, 0.5f, 0.5f,		0, 0, 1,
		0.5f, 0.5f, 0.5f,		0, 1, 0,
		-0.5f, -0.5f, 0.5f,		1, 0, 0,
		0.5f, -0.5f, 0.5f,		0, 1, 1,
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

	m_pVertexBuffer.reset(Smile::VertexBuffer::Create(Smile::SmileGame::GetInstance().GetWindow().GetRenderingContext(), vertices, 8, bufferLayout));

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
	m_pIndexBuffer.reset(Smile::IndexBuffer::Create(Smile::SmileGame::GetInstance().GetWindow().GetRenderingContext(), indices, 36));

	m_pShader.reset(Smile::Shader::Create(Smile::SmileGame::GetInstance().GetWindow().GetRenderingContext(), "../SmileProject/Resources/shaders/PosCol3D.fx", bufferLayout));

	m_pActiveScene.reset(new Smile::Scene{});
	auto triangle = m_pActiveScene->CreateEntity("Triangle");

	Smile::MeshRendererComponent::MeshRendererData meshRendererData{};
	meshRendererData.pVertices = vertices;
	meshRendererData.VertexCount = 3;
	meshRendererData.pIndices = indices;
	meshRendererData.IndexCount = 3;
	meshRendererData.ShaderFilePath = "../SmileProject/Resources/shaders/PosCol3D.fx";
	meshRendererData.BufferLayout = {
		{ Smile::ShaderDataType::eFloat3, "Position" },
		{ Smile::ShaderDataType::eFloat3, "Color" }
	};

	triangle.AddComponent<Smile::MeshRendererComponent>(Smile::SmileGame::GetInstance().GetWindow().GetRenderingContext(), meshRendererData);
}

void ExampleLayer::OnUpdate(Smile::Timestep deltaTime)
{
	if (Smile::Input::IsKeyPressed(SM_LEFT))
		m_CameraPosition.x -= m_CameraMoveSpeed * deltaTime;
	if (Smile::Input::IsKeyPressed(SM_RIGHT))
		m_CameraPosition.x += m_CameraMoveSpeed * deltaTime;
	if (Smile::Input::IsKeyPressed(SM_DOWN))
		m_CameraPosition.y -= m_CameraMoveSpeed * deltaTime;
	if (Smile::Input::IsKeyPressed(SM_UP))
		m_CameraPosition.y += m_CameraMoveSpeed * deltaTime;

	if (Smile::Input::IsKeyPressed('A'))
		m_CameraRotation += m_CameraRotationSpeed * deltaTime;
	if (Smile::Input::IsKeyPressed('D'))
		m_CameraRotation -= m_CameraRotationSpeed * deltaTime;

	Smile::RenderingContext* pRenderingContext = Smile::SmileGame::GetInstance().GetWindow().GetRenderingContext();

	Smile::RenderCommand::SetClearColor({ DirectX::Colors::DodgerBlue.f[0], DirectX::Colors::DodgerBlue.f[1], DirectX::Colors::DodgerBlue.f[2], DirectX::Colors::DodgerBlue.f[3] });
	Smile::RenderCommand::Clear(pRenderingContext);

	m_Camera.SetPosition(m_CameraPosition);
	m_Camera.SetRotation(m_CameraRotation);

	Smile::Renderer::BeginScene(m_Camera);

	/*static DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(0.1f, 0.1f, 1);

	for (int i{}; i < 20; ++i)
	{
		for (int j{}; j < 20; ++j)
		{
			DirectX::XMFLOAT3 pos{ j * 0.11f, i * 0.11f, 0 };
			DirectX::XMMATRIX worldTransformMat = DirectX::XMMatrixMultiply(scaleMat, DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z));
			DirectX::XMFLOAT4X4 worldTransform{};
			DirectX::XMStoreFloat4x4(&worldTransform, worldTransformMat);

			m_pShader->UploadFloat3("Color", m_TriangleColor);
			

			Smile::Renderer::Submit(pRenderingContext, m_pVertexBuffer, m_pIndexBuffer, m_pShader, worldTransform);
		}
	}*/

	m_pActiveScene->OnUpdate(deltaTime);

	Smile::Renderer::EndScene();
}

void ExampleLayer::OnEvent(Smile::Event& event)
{
	Smile::EventDispatcher dispatcher{ event };
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Triangle Color", &m_TriangleColor.x);
	ImGui::End();
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