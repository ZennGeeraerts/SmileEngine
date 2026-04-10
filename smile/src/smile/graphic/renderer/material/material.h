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
 * @file        material_asset.h
 * @author      Zenn Geeraerts
 * @created     9 Januari 2026
 * @brief       Asset for material
 */
#pragma once

#include "smile/core/asset/asset.h"
#include "smile/graphic/renderer/shader/program.h"
#include "smile/graphic/sprite/texture_asset.h"
#include "material_instance.h"

#include <DirectXMath.h>

#include <variant>

namespace smile::graphic
{
    enum class MaterialParameterType
    {
        Float,
        Int,
        Bool,
        Float2,
        Float3,
    };

    struct MaterialLayout final
    {
        struct Parameter final
        {
            Parameter( const primitive::String &name, MaterialParameterType type, Uint32 offset, Count size ) noexcept
                : Name{ name }, Type{ type }, Offset{ offset }, Size{ size }
            {
            }

            primitive::String Name;
            MaterialParameterType Type;
            Uint32 Offset;
            Count Size;

            inline bool operator==( const Parameter &other ) const noexcept
            {
                return Name == other.Name && Type == other.Type && Offset == other.Offset && Size == other.Size;
            }
        };

        struct Texture final
        {
            primitive::String Name;
            Uint32 Slot;

            inline bool operator==( const Texture &other ) const noexcept
            {
                return Name == other.Name && Slot == other.Slot;
            }
        };

        primitive::Vector< Parameter > Parameters;
        primitive::Vector< Texture > Textures;
        Uint32 CbSlot;
        Count CbSize;
        foundation::Flags< rhi::ShaderStage > Visibility;
    };

    class Material final : public asset::Asset
    {
      public:
        using Ref = memory::Ref< Material >;
        using ConstRef = memory::Ref< const Material >;

        Material( const primitive::String &name, const MaterialLayout &layout );

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< Material >() };
        }

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

    void
    BuildMaterialLayoutAndDescriptor( Program::ConstRef program, MaterialLayout &layout, MaterialDescriptor &desc );
}