#include "smpch.h"
#include "imgui_layer.h"

#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_impl_win32.h"
#include "thirdparty/imgui/imgui_impl_dx11.h"
#include "thirdparty/imguizmo/ImGuizmo.h"

#include "smile_engine/core/application.h"
#include "smile_engine/core/window.h"
#include "smile_engine/renderer/renderer_api.h"

#include "platform/directx11/directx11_context.h"

#include "smile_engine/core/logger.h"
#include "smile_engine/input/key_codes.h"

namespace smile::imgui
{
    ImGuiLayer::ImGuiLayer() : Layer( "ImGuiLayer" )
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
        shutDown();
    }

    void ImGuiLayer::shutDown()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::onAttach()
    {
        SM_ASSERT( !initialized, "ImGui is initialized more than once. Only initialize it 1 time!" );

        ImGui_ImplWin32_EnableDpiAwareness();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO &io = ImGui::GetIO();
        ( void )io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        // io.ConfigViewportsNoAutoMerge = true;
        // io.ConfigViewportsNoTaskBarIcon = true;
        // io.ConfigViewportsNoDefaultParent = true;
        // io.ConfigDockingAlwaysTabBar = true;
        // io.ConfigDockingTransparentPayload = true;

        const float fontSize = 18.f;
        io.FontDefault = io.Fonts->AddFontFromFileTTF( "assets/fonts/Heebo/Heebo-Regular.ttf", fontSize );
        io.Fonts->AddFontFromFileTTF( "assets/fonts/Heebo/Heebo-Bold.ttf", fontSize );

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
        // ones.
        ImGuiStyle &style = ImGui::GetStyle();
        if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        setDarkThemeColors();

        Window &window = Application::getInstance().getWindow();
        renderer::GraphicsContext *graphicsContext = window.getGraphicsContext();

        renderer::RendererAPI::API api = renderer::RendererAPI::getAPI();
        switch ( api )
        {
            case renderer::RendererAPI::API::DirectX11:
            {
                ImGui_ImplWin32_Init( window.getNativeWindow() );

                renderer::DirectX11Context *directx11_context = static_cast< renderer::DirectX11Context * >( graphicsContext );
                ImGui_ImplDX11_Init( directx11_context->getDevice(), directx11_context->getDeviceContext() );
                break;
            }
            case renderer::RendererAPI::API::SmileRaster:
                break;

            default:
                break;
        }

        initialized = true;
    }

    void ImGuiLayer::onDetach()
    {
        shutDown();
    }

    void ImGuiLayer::begin()
    {
        renderer::RendererAPI::API api = renderer::RendererAPI::getAPI();
        switch ( api )
        {
            case renderer::RendererAPI::API::DirectX11:
                ImGui_ImplDX11_NewFrame();
                ImGui_ImplWin32_NewFrame();
                break;

            case renderer::RendererAPI::API::SmileRaster:
                return;

            default:
                return;
        }

        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO &io = ImGui::GetIO();
        Window &window = Application::getInstance().getWindow();
        io.DisplaySize =
            ImVec2{ static_cast< float >( window.getWidth() ), static_cast< float >( window.getHeight() ) };

        renderer::RendererAPI::API api = renderer::RendererAPI::getAPI();
        switch ( api )
        {
            case renderer::RendererAPI::API::DirectX11:
                ImGui::Render();
                ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
                break;

            case renderer::RendererAPI::API::SmileRaster:
                return;

            default:
                return;
        }

        // Update and Render additional Platform Windows
        if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void ImGuiLayer::onImGuiRender()
    {
        /*static bool bShow = true;
        ImGui::ShowDemoWindow(&bShow);*/
    }

    void ImGuiLayer::onEvent( Event &event )
    {
        EventDispatcher dispatcher{ event };
        dispatcher.dispatch< MouseButtonPressedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::onMouseButtonPressed ) );
        dispatcher.dispatch< MouseButtonReleasedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::onMouseButtonReleased ) );
        dispatcher.dispatch< MouseMovedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::onMouseMoved ) );
        dispatcher.dispatch< MouseScrolledEvent >( SM_BIND_EVENT_FN( ImGuiLayer::onMouseScrolled ) );
        dispatcher.dispatch< KeyPressedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::onKeyPressed ) );
        dispatcher.dispatch< KeyReleasedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::onKeyReleased ) );
        dispatcher.dispatch< KeyTypedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::onKeyTyped ) );
        dispatcher.dispatch< WindowResizeEvent >( SM_BIND_EVENT_FN( ImGuiLayer::onWindowResize ) );
    }

    bool ImGuiLayer::onMouseButtonPressed( MouseButtonPressedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[e.getMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::onMouseButtonReleased( MouseButtonReleasedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[e.getMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::onMouseMoved( MouseMovedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2{ e.getX(), e.getY() };

        return false;
    }

    bool ImGuiLayer::onMouseScrolled( MouseScrolledEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheel += e.getOffsetX();
        io.MouseWheelH += e.getOffsetY();

        return false;
    }

    bool ImGuiLayer::onKeyPressed( KeyPressedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[e.getKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[input::key::CtrlLeft] || io.KeysDown[input::key::CtrlRight];
        io.KeyShift = io.KeysDown[input::key::ShiftLeft] || io.KeysDown[input::key::ShiftRight];
        io.KeyAlt = io.KeysDown[input::key::AltLeft] || io.KeysDown[input::key::AltRight];
        io.KeySuper = io.KeysDown[input::key::WindowsLeft] || io.KeysDown[input::key::WindowsRight];
        return false;
    }

    bool ImGuiLayer::onKeyReleased( KeyReleasedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[e.getKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::onKeyTyped( KeyTypedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        int key_code = e.getKeyCode();
        if ( ( key_code > 0 ) && ( key_code < 0x10000 ) )
            io.AddInputCharacter( static_cast< unsigned short >( key_code ) );

        return false;
    }

    bool ImGuiLayer::onWindowResize( WindowResizeEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2{ static_cast< float >( e.getWidth() ), static_cast< float >( e.getHeight() ) };
        io.DisplayFramebufferScale = ImVec2{ 1.f, 1.f };

        return false;
    }

    void ImGuiLayer::setDarkThemeColors()
    {
        auto &colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.663f, 0.2f, 0.227f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.663f, 0.2f, 0.227f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.663f, 0.2f, 0.227f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.663f, 0.2f, 0.227f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }
}