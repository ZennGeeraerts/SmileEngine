/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smpch.h"
#include "smile_engine/core/core.h"
#include "smile_engine/core/events/event.h"

namespace smile
{
    struct WindowSettings final
    {
        WindowSettings( const std::string &title = "Smile Engine", Uint32 width = 1280, Uint32 height = 720 )
            : Title{ title }, Width{ width }, Height{ height }
        {
        }

        std::string Title;
        Uint32 Width;
        Uint32 Height;
    };

    namespace graphic
    {
        class GraphicsDevice;
        class GraphicsContext;
    }

    // Window interface for desktop platforms
    class Window
    {
      public:
        using EventCallbackFunction = std::function< void( Event & ) >;

        Window() = default;
        virtual ~Window()
        {
        }
        Window( const Window & ) = delete;
        Window( Window && ) = delete;
        Window &operator=( const Window & ) = delete;
        Window &operator=( Window && ) = delete;

        virtual void OnUpdate() = 0;

        Uint32 GetWidth() const
        {
            return m_Data.Settings.Width;
        }
        Uint32 GetHeight() const
        {
            return m_Data.Settings.Height;
        }
        graphic::GraphicsDevice* GetGraphicsDevice() const
        {
            return m_pDevice;
        }
        graphic::GraphicsContext* GetGraphicsContext() const
        {
            return m_pContext;
        }

        // Window attributes
        void SetEventCallback(const EventCallbackFunction& callback)
        {
            m_Data.EventCallback = callback;
        }
        void SetVSync(bool isEnabled)
        {
            m_Data.IsVSync = isEnabled;
        }
        bool IsVSync() const 
        {
            return m_Data.IsVSync;
        }

        // Returns the child window
        virtual void *GetNativeWindow() const = 0;

      protected:
        struct WindowData
        {
            WindowSettings Settings{};
            bool IsVSync{};
            EventCallbackFunction EventCallback{};
        };

        WindowData m_Data;
        bool m_IsInitialized = false;

        graphic::GraphicsDevice *m_pDevice;
        graphic::GraphicsContext *m_pContext;
    };
}
