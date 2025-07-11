/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/rhi/render_handle.h"

namespace smile::graphic
{
    class GraphicsPipeline final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< GraphicsPipeline >;

        GraphicsPipeline( GraphicsPipelineHandle handle,
            PrimitiveTopology topology,
            const BufferLayout &inputLayout,
            const RenderState &renderState,
            VertexShader::ConstRef pVertexShader,
            PixelShader::ConstRef pPixelShader,
            const primitive::Vector< BindingLayout > &bindingLayouts )
            : m_Handle{ handle },
              m_PrimitiveTopology{ topology },
              m_InputLayout{ inputLayout },
              m_RenderState{ renderState },
              m_pVertexShader{ pVertexShader },
              m_pPixelShader{ pPixelShader },
              m_BindingLayouts{ bindingLayouts }
        {
        }

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

      private:
        GraphicsPipelineHandle m_Handle;

        PrimitiveTopology m_PrimitiveTopology;
        BufferLayout m_InputLayout;
        RenderState m_RenderState;

        VertexShader::ConstRef m_pVertexShader;
        PixelShader::ConstRef m_pPixelShader;

        primitive::Vector< BindingLayout > m_BindingLayouts;

        friend class ResourceManager;
    };
}