#pragma once

#include "smile_engine/graphic/resource/rasterizer_state.h"
#include "platform/directX11/directx11_context.h"

namespace smile::graphic
{
    class DirectX11RasterizerState final : public RasterizerState
    {
      public:
        DirectX11RasterizerState( const RasterizerStateDescriptor &rasterizerStateDesc );
        virtual ~DirectX11RasterizerState();

        DirectX11RasterizerState( const DirectX11RasterizerState & ) = delete;
        DirectX11RasterizerState( DirectX11RasterizerState && ) = delete;
        DirectX11RasterizerState &operator=( const DirectX11RasterizerState & ) = delete;
        DirectX11RasterizerState &operator=( DirectX11RasterizerState && ) = delete;

        virtual void Bind() const override;
        virtual void Unbind() const override;

      private:
        DirectX11Context *m_pDirectX11Context = nullptr;
        ID3D11RasterizerState *m_pRasterizerState = nullptr;
    };
}