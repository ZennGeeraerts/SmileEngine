#include "MainGame.h"

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
	float vertices[]
	{
		0.f, 0.5f, 0.5f,		0.8f, 0.2f, 0.8f,
		0.5f, -0.5f, 0.5f,		0.2f, 0.8f, 0.8f,
		-0.5f, -0.5f, 0.5f,		0.8f, 0.8f, 0.2f
	};

	Smile::BufferLayout bufferLayout
	{
		{ Smile::ShaderDataType::eFloat3, "Position" },
		{ Smile::ShaderDataType::eFloat3, "Color" }
	};

	m_pVertexBuffer.reset(Smile::VertexBuffer::Create(Smile::SmileGame::GetInstance().GetWindow().GetRenderingContext(), vertices, 3, bufferLayout));

	uint32_t indices[]{ 0, 1, 2 };
	m_pIndexBuffer.reset(Smile::IndexBuffer::Create(Smile::SmileGame::GetInstance().GetWindow().GetRenderingContext(), indices, 3));

	m_pShader.reset(Smile::Shader::Create(Smile::SmileGame::GetInstance().GetWindow().GetRenderingContext(), "../SmileProject/Resources/shaders/PosCol3D.fx", bufferLayout));
}

void ExampleLayer::OnUpdate(Smile::Timestep deltaTime)
{
	SM_LOG_TRACE("Delta time: %.6f (%.6f ms)", deltaTime.GetSeconds(), deltaTime.GetMilliseconds());
	SM_LOG_TRACE("FPS: %d", Smile::SmTime::GetInstance().GetFPS());

	if(Smile::Input::IsKeyPressed(SM_LEFT))
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
	Smile::Renderer::Submit(pRenderingContext, m_pVertexBuffer, m_pIndexBuffer, m_pShader);
	Smile::Renderer::EndScene();
}

void ExampleLayer::OnEvent(Smile::Event& event)
{
	Smile::EventDispatcher dispatcher{ event };
}

void ExampleLayer::OnImGuiRender()
{

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