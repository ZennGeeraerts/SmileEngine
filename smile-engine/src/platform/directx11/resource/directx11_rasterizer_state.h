#pragma once

#include "smile_engine/renderer/resource/rasterizer_state.h"
#include "platform/directX11/directx11_context.h"

namespace smile::renderer
{
    class DirectX11RasterizerState final : public RasterizerState
    {
      public:
        DirectX11RasterizerState( const RasterizerStateData &rasterizer_state_data );
        virtual ~DirectX11RasterizerState();

        DirectX11RasterizerState( const DirectX11RasterizerState & ) = delete;
        DirectX11RasterizerState( DirectX11RasterizerState && ) = delete;
        DirectX11RasterizerState &operator=( const DirectX11RasterizerState & ) = delete;
        DirectX11RasterizerState &operator=( DirectX11RasterizerState && ) = delete;

        virtual void bind() const override;
        virtual void unbind() const override;

      private:
        DirectX11Context *directX11Context = nullptr;
        ID3D11RasterizerState *rasterizerState = nullptr;
    };
}