#include "smpch.h"
#include "ImGuiLayer.h"

#include "Libs/ImGui/imgui.h"
#include "Libs/ImGui/imgui_impl_win32.h"
#include "Libs/ImGui/imgui_impl_dx11.h"
#include "Libs/ImGuizmo/ImGuizmo.h"

#include "SmileEngine/Core/SmileGame.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/DirectX11/DirectX11Context.h"

#include "SmileEngine/Core/Logger.h"
#include "SmileEngine/Core/KeyCodes.h"


namespace Smile
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
		ShutDown();
	}

	void ImGuiLayer::ShutDown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnAttach()
	{
		SM_ASSERT(!m_bInitialized, "ImGui is initialized more than once. Only initialize it 1 time!");

		ImGui_ImplWin32_EnableDpiAwareness();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		//io.ConfigViewportsNoDefaultParent = true;
		//io.ConfigDockingAlwaysTabBar = true;
		//io.ConfigDockingTransparentPayload = true;

		const float fontSize = 18.f;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Heebo/Heebo-Regular.ttf", fontSize);
		io.Fonts->AddFontFromFileTTF("Resources/Fonts/Heebo/Heebo-Bold.ttf", fontSize);

		 // Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		 // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Window& window = SmileGame::GetInstance().GetWindow();
		ImGui_ImplWin32_Init(window.GetNativeWindow());

		RenderingContext* pRenderingContext = window.GetRenderingContext();
		DirectX11Context* pD11Context = static_cast<DirectX11Context*>(pRenderingContext);

		SM_ASSERT(pD11Context, "ImGuiLayer::OnAttach > ImGuiLayer currently only supports DirectX11");
		ImGui_ImplDX11_Init(pD11Context->GetDevice(), pD11Context->GetDeviceContext());

		m_bInitialized = true;
	}

	void ImGuiLayer::OnDetach()
	{
		ShutDown();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Window& window = SmileGame::GetInstance().GetWindow();
		io.DisplaySize = ImVec2{ static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()) };

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		/*static bool bShow = true;
		ImGui::ShowDemoWindow(&bShow);*/
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher{ event };
		dispatcher.Dispatch<MouseButtonPressedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleased));
		dispatcher.Dispatch<MouseMovedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnMouseMoved));
		dispatcher.Dispatch<MouseScrolledEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolled));
		dispatcher.Dispatch<KeyPressedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnKeyReleased));
		dispatcher.Dispatch<KeyTypedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnKeyTyped));
		dispatcher.Dispatch<WindowResizeEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnWindowResize));
	}

	bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMoved(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2{ e.GetX(), e.GetY() };

		return false;
	}

	bool ImGuiLayer::OnMouseScrolled(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetOffsetX();
		io.MouseWheelH += e.GetOffsetY();

		return false;
	}

	bool ImGuiLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[SM_LCONTROL] || io.KeysDown[SM_RCONTROL];
		io.KeyShift = io.KeysDown[SM_LSHIFT] || io.KeysDown[SM_RSHIFT];
		io.KeyAlt = io.KeysDown[SM_LMENU] || io.KeysDown[SM_RMENU];
		io.KeySuper = io.KeysDown[SM_LWIN] || io.KeysDown[SM_RWIN];
		return false;
	}

	bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTyped(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if ((keycode > 0) && (keycode < 0x10000))
			io.AddInputCharacter(static_cast<unsigned short>(keycode));

		return false;
	}

	bool ImGuiLayer::OnWindowResize(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2{ static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()) };
		io.DisplayFramebufferScale = ImVec2{ 1.f, 1.f };

		return false;
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.658f, 0.874f, 0.396f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.658f, 0.874f, 0.396f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.658f, 0.874f, 0.396f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.658f, 0.874f, 0.396f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}
}