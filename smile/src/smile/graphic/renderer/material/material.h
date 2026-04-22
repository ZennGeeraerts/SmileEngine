/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        material.h
 * @author      Zenn Geeraerts
 * @created     9 Januari 2026
 * @brief       Holds the data of a material and its default instance
 */
#pragma once

#include "material_layout.h"
#include "material_instance.h"

namespace smile::graphic
{
    class TextureManager;

    class Material final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Material >;
        using ConstRef = memory::Ref< const Material >;

        Material( const primitive::String &name, const MaterialLayout &layout );
        ~Material() = default;

        primitive::StringView GetName() const
        {
            return m_Name.AsStringView();
        }

        const MaterialLayout &GetLayout() const
        {
            return m_Layout;
        }

        MaterialInstance::Ref GetDefaultInstance() const
        {
            return m_DefaultInstance;
        }

        void SetParameter( const primitive::StringView name, const MaterialParameterValue &value );
        MaterialParameterValue GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name,
            Texture::ConstRef texture,
            const rhi::SamplerDescriptor &samplerDesc );

        const MaterialTextureBinding &GetTextureBinding( const primitive::StringView name ) const;

      private:
        primitive::String m_Name;
        MaterialLayout m_Layout;
        MaterialInstance::Ref m_DefaultInstance = nullptr;

        friend class MaterialSystem;
    };

    void BuildMaterialLayoutAndDescriptor( const TextureManager &textureManager,
        Program::ConstRef program,
        MaterialLayout &layout,
        MaterialDescriptor &desc );
}