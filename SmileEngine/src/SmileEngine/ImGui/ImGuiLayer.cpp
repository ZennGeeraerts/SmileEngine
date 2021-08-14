#include "smpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

#include "SmileEngine/SmileGame.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/DirectX11/DirectX11Context.h"

#include "SmileEngine/Logger.h"

namespace Smile
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should eventually use Smile Engine keycodes
		io.KeyMap[ImGuiKey_Tab] = VK_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
		io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
		io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
		io.KeyMap[ImGuiKey_Home] = VK_HOME;
		io.KeyMap[ImGuiKey_End] = VK_END;
		io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
		io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
		io.KeyMap[ImGuiKey_Space] = VK_SPACE;
		io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
		io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = VK_RETURN;
		io.KeyMap[ImGuiKey_A] = 'A';
		io.KeyMap[ImGuiKey_C] = 'C';
		io.KeyMap[ImGuiKey_V] = 'V';
		io.KeyMap[ImGuiKey_X] = 'X';
		io.KeyMap[ImGuiKey_Y] = 'Y';
		io.KeyMap[ImGuiKey_Z] = 'Z';

		Window& window = SmileGame::GetInstance().GetWindow();
		try 
		{
			WindowsWindow& windowsWindow = dynamic_cast<WindowsWindow&>(window);
			ImGui_ImplWin32_Init(windowsWindow.GetWindowHandle());

			RenderingContext* pRenderingContext = windowsWindow.GetRenderingContext();
			DirectX11Context* pD11Context = dynamic_cast<DirectX11Context*>(pRenderingContext);

			if (pD11Context)
				ImGui_ImplDX11_Init(pD11Context->GetDevice(), pD11Context->GetDeviceContext());
			else
				SM_ERROR("ImGuiLayer::OnAttach > ImGuiLayer currently only supports DirectX11");	
		}
		catch (const std::bad_cast& exc) 
		{
			SM_ERROR("ImGuiLayer::OnAttach > The current window is not a WindowsWindow");
		}
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Window& window = SmileGame::GetInstance().GetWindow();
		io.DisplaySize = ImVec2{ static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()) };
		io.DeltaTime = SmTime::GetInstance().GetDeltaTime();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		static bool bShow = true;
		ImGui::ShowDemoWindow(&bShow);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{

	}
}