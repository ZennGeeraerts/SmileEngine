/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/handle.h"
#include "smile/common/foundation/flags.h"
#include "smile/common/memory/counted.h"
#include "smile/graphic/renderer/shader/program.h"
#include "smile/graphic/renderer/resource/texture.h"

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
        Float4,
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

            inline bool operator==( const Parameter &other ) const
            {
                return Name == other.Name && Type == other.Type && Offset == other.Offset && Size == other.Size;
            }
        };

        struct Texture final
        {
            Texture( const primitive::String &name, Uint32 slot ) noexcept : Name{ name }, Slot{ slot }
            {
            }

            primitive::String Name;
            Uint32 Slot;

            inline bool operator==( const Texture &other )
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

    using MaterialParameterValue = std::
        variant< bool, int, float, DirectX::XMFLOAT2, DirectX::XMFLOAT3, DirectX::XMFLOAT4, primitive::Vector< Byte > >;

    struct MaterialDescriptor final
    {
        Program::ConstRef ShaderProgram;
        primitive::HashMap< primitive::String, MaterialParameterValue > Parameters;
        primitive::HashMap< primitive::String, Texture::ConstRef > TextureBindings;
    };

    class Material final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Material >;
        using ConstRef = memory::Ref< const Material >;

        using ID = primitive::Handle< Uint32, 24, 8 >;

        enum class DirtyFlags
        {
            Parameter,
            Texture,
        };

        Material( ID id, const MaterialLayout &layout, const MaterialDescriptor &desc );
        ~Material() = default;

        void Clear();

        void SetParameter( const primitive::StringView name, const MaterialParameterValue &value );
        MaterialParameterValue GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name, Texture::ConstRef texture );
        Texture::ConstRef GetTextureBinding( const primitive::StringView name ) const;

        const MaterialLayout &GetLayout() const
        {
            return m_Layout;
        }

        const MaterialDescriptor &GetDescriptor() const
        {
            return m_Descriptor;
        }

        ID GetID() const
        {
            return m_ID;
        }

        foundation::Flags< DirtyFlags > GetDirtyFlags() const
        {
            return m_DirtyFlags;
        }

        void ClearDirtyFlags()
        {
            m_DirtyFlags.ClearAll();
        }

      private:
        MaterialLayout m_Layout;
        MaterialDescriptor m_Descriptor;
        ID m_ID;

        foundation::Flags< DirtyFlags > m_DirtyFlags;
    };
}
