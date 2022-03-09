#pragma once

#include "SmileEngine/Renderer/Resource/RasterizerState.h"
#include "Platform/DirectX11/DirectX11Context.h"

namespace smile
{
    class DirectX11RasterizerState final : public RasterizerState
    {
      public:
        DirectX11RasterizerState( const RasterizerStateData &rasterizerStateData );
        virtual ~DirectX11RasterizerState();

        virtual void Bind() const override;
        virtual void Unbind() const override;

      private:
        DirectX11Context *m_pDirectX11Context = nullptr;
        ID3D11RasterizerState *m_pRasterizerState = nullptr;
    };
}