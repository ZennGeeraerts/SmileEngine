/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "material_instance.h"
#include "smile/graphic/renderer/shader/vertex_shader.h"
#include "smile/graphic/renderer/shader/pixel_shader.h"
#include "smile/graphic/shader/constant_buffer_descriptor.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/hash_map.h"

namespace smile::graphic
{
    class Material final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Material >;
        using ConstRef = memory::Ref< const Material >;

        Material( MaterialInstance::ConstRef pMaterialInstance );
        ~Material() noexcept;

        inline VertexShader::ConstRef GetVertexShader() const
        {
            return m_pMaterialInstance->GetSh;
        }

        inline PixelShader::ConstRef GetPixelShader() const
        {
            return m_pPixelShader;
        }

      private:
        MaterialInstance::ConstRef m_pMaterialInstance;
    };
}
