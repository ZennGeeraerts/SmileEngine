/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "imgui_layer.h"

#include "smile/engine/core/application/application.h"
#include "smile/engine/core/window/window.h"
#include "smile/engine/graphic/renderer/render_command.h"

#include "smile/platform/directx11/graphic/directx11_device.h"
#include "smile/platform/directx11/graphic/directx11_context.h"

#include "smile/engine/common/logger/logger.h"
#include "smile/engine/core/input/key_codes.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <ImGuizmo.h>

namespace smile::imgui
{
    ImGuiLayer::ImGuiLayer() : Layer( "ImGuiLayer" )
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
        SM_ASSERT( !m_IsInitialized, "ImGui is initialized more than once. Only initialize it 1 time!" );

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
        io.FontDefault = io.Fonts->AddFontFromFileTTF( "resources/fonts/Heebo/Heebo-Regular.ttf", fontSize );
        io.Fonts->AddFontFromFileTTF( "resources/fonts/Heebo/Heebo-Bold.ttf", fontSize );

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

        SetDarkThemeColors();

        window::Window &window = application::Application::GetInstance().GetMainWindow();
        graphic::GraphicsDevice *pGraphicsDevice = graphic::RenderCommand::GetGraphicsDevice();
        graphic::GraphicsContext *pGraphicsContext = graphic::RenderCommand::GetGraphicsContext();

        graphic::RendererAPI::API api = graphic::RendererAPI::GetAPI();
        switch ( api )
        {
            case graphic::RendererAPI::API::DirectX11:
            {
                ImGui_ImplWin32_Init( window.GetNativeWindow() );

                ID3D11Device *pDevice = static_cast< ID3D11Device * >( pGraphicsDevice->GetInternal() );
                ID3D11DeviceContext *pDeviceContext =
                    static_cast< ID3D11DeviceContext * >( pGraphicsContext->GetInternal() );

                ImGui_ImplDX11_Init( pDevice, pDeviceContext );
                break;
            }
            case graphic::RendererAPI::API::SmileRaster:
                break;

            default:
                break;
        }

        m_IsInitialized = true;
    }

    void ImGuiLayer::OnDetach()
    {
        ShutDown();
    }

    void ImGuiLayer::Begin()
    {
        graphic::RendererAPI::API api = graphic::RendererAPI::GetAPI();
        switch ( api )
        {
            case graphic::RendererAPI::API::DirectX11:
                ImGui_ImplDX11_NewFrame();
                ImGui_ImplWin32_NewFrame();
                break;

            case graphic::RendererAPI::API::SmileRaster:
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
        window::Window &window = application::Application::GetInstance().GetMainWindow();
        io.DisplaySize =
            ImVec2{ static_cast< float >( window.GetWidth() ), static_cast< float >( window.GetHeight() ) };

        ImGui::Render();

        graphic::RendererAPI::API api = graphic::RendererAPI::GetAPI();
        switch ( api )
        {
            case graphic::RendererAPI::API::DirectX11:
                ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
                break;

            case graphic::RendererAPI::API::SmileRaster:
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

    void ImGuiLayer::OnImGuiRender()
    {
        /*static bool bShow = true;
        ImGui::ShowDemoWindow(&bShow);*/
    }

    void ImGuiLayer::OnEvent( window::Event &event )
    {
        window::EventDispatcher dispatcher{ event };
        dispatcher.Dispatch< window::MouseButtonPressedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::OnMouseButtonPressed ) );
        dispatcher.Dispatch< window::MouseButtonReleasedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::OnMouseButtonReleased ) );
        dispatcher.Dispatch< window::MouseMovedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::OnMouseMoved ) );
        dispatcher.Dispatch< window::MouseScrolledEvent >( SM_BIND_EVENT_FN( ImGuiLayer::OnMouseScrolled ) );
        dispatcher.Dispatch< window::KeyPressedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::OnKeyPressed ) );
        dispatcher.Dispatch< window::KeyReleasedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::OnKeyReleased ) );
        dispatcher.Dispatch< window::KeyTypedEvent >( SM_BIND_EVENT_FN( ImGuiLayer::OnKeyTyped ) );
        dispatcher.Dispatch< window::WindowResizeEvent >( SM_BIND_EVENT_FN( ImGuiLayer::OnWindowResize ) );
    }

    bool ImGuiLayer::OnMouseButtonPressed( window::MouseButtonPressedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleased( window::MouseButtonReleasedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseMoved( window::MouseMovedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2{ e.GetX(), e.GetY() };

        return false;
    }

    bool ImGuiLayer::OnMouseScrolled( window::MouseScrolledEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheel += e.GetOffsetX();
        io.MouseWheelH += e.GetOffsetY();

        return false;
    }

    bool ImGuiLayer::OnKeyPressed( window::KeyPressedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[input::key::CtrlLeft] || io.KeysDown[input::key::CtrlRight];
        io.KeyShift = io.KeysDown[input::key::ShiftLeft] || io.KeysDown[input::key::ShiftRight];
        io.KeyAlt = io.KeysDown[input::key::AltLeft] || io.KeysDown[input::key::AltRight];
        io.KeySuper = io.KeysDown[input::key::WindowsLeft] || io.KeysDown[input::key::WindowsRight];
        return false;
    }

    bool ImGuiLayer::OnKeyReleased( window::KeyReleasedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::OnKeyTyped( window::KeyTypedEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        int keyCode = e.GetKeyCode();
        if ( ( keyCode > 0 ) && ( keyCode < 0x10000 ) )
            io.AddInputCharacter( static_cast< unsigned short >( keyCode ) );

        return false;
    }

    bool ImGuiLayer::OnWindowResize( window::WindowResizeEvent &e )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2{ static_cast< float >( e.GetWidth() ), static_cast< float >( e.GetHeight() ) };
        io.DisplayFramebufferScale = ImVec2{ 1.f, 1.f };

        return false;
    }

    void ImGuiLayer::SetDarkThemeColors()
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