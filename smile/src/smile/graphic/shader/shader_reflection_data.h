/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/hash_map.h"
#include "smile/common/primitive/text/string.h"

#include "shader_blob_format.h"
#include "constant_buffer_descriptor.h"
#include "smile/graphic/rhi/resource/buffer.h"
#include "smile/graphic/rhi/shader/binding_layout.h"

namespace smile::graphic
{
    enum class ResourceBindingType
    {
        Unknown,
        Buffer,
        Texture,
        Sampler
    };

    ResourceBindingType ResourceTypeToBindingType( rhi::ResourceType resType );

    struct ResourceBindingKey final
    {
        ResourceBindingKey() = default;

        ResourceBindingKey( Uint32 slot, ResourceBindingType type ) : Slot{ slot }, Type{ type }
        {
        }

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( Slot ) );
            hash = foundation::HashCombine( hash, std::hash< ResourceBindingType >{}( Type ) );
            return hash;
        }

        inline bool operator==( const ResourceBindingKey &other ) const
        {
            return Slot == other.Slot && Type == other.Type;
        }

        Uint32 Slot;
        ResourceBindingType Type;
    };

    struct NamedBindingLayoutElement final
    {
        NamedBindingLayoutElement( const rhi::BindingLayoutElement &element, const primitive::String &name )
            : Element{ element }, Name{ name }
        {
        }

        rhi::BindingLayoutElement Element;
        primitive::String Name;
    };

    struct ShaderReflectionData final
    {
        primitive::String EntryPoint;
        primitive::String TargetProfile;
        ShaderBlobFormat BlobFormat;
        rhi::BufferLayout InputSignature;
        rhi::BufferLayout OutputSignature;
        primitive::HashMap< ResourceBindingKey, NamedBindingLayoutElement > ShaderResourceBindings;
        primitive::HashMap< primitive::String, ConstantBufferDescriptor > ConstantBufferDescs;
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::ResourceBindingKey >
    {
        smile::foundation::HashCode operator()(
            const smile::graphic::ResourceBindingKey &bufferBindingKey ) const noexcept
        {
            return bufferBindingKey.GetHashCode();
        }
    };
}