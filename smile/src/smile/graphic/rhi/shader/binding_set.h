/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "resource_type.h"
#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/rhi/resource/buffer.h"
#include "smile/graphic/rhi/resource/texture.h"

namespace smile::graphic::rhi
{
    struct BindingSetElement final
    {
        BindingSetElement(){};

        ResourceType Type;
        union
        {
            TextureHandle Texture;
            GPUBufferHandle Buffer;
            SamplerHandle Sampler;
        };
        Uint32 Slot;

        Format BindingFormat = Format::UNKNOWN;                 // Valid for textures, buffer srv and buffer uav
        TextureDimension Dimension = TextureDimension::Unknown; // Valid for texture srv and texture uav

        union
        {
            TextureSubresourceSet Subresources; // Valid for texture srv and texture uav
            BufferRange Range;                  // Valid for buffers
            Uint32 RawData[2];
        };

        static BindingSetElement CreateUnknown( Uint32 slot = 0 )
        {
            BindingSetElement result;
            result.Type = ResourceType::Unknown;
            result.Texture = TextureHandle::NullHandle();
            result.Slot = slot;
            result.BindingFormat = Format::UNKNOWN;
            result.Dimension = TextureDimension::Unknown;
            result.RawData[0] = 0;
            result.RawData[1] = 0;
            return result;
        }

        static BindingSetElement CreateTextureSRV( Uint32 slot,
            TextureHandle texture,
            Format format = Format::UNKNOWN,
            const TextureSubresourceSet &subresources = s_AllSubresources,
            TextureDimension dimension = TextureDimension::Unknown )
        {
            BindingSetElement result;
            result.Type = ResourceType::Texture_SRV;
            result.Texture = texture;
            result.Slot = slot;
            result.BindingFormat = format;
            result.Dimension = dimension;
            result.Subresources = subresources;
            return result;
        }

        static BindingSetElement CreateTextureUAV( Uint32 slot,
            TextureHandle texture,
            Format format = Format::UNKNOWN,
            const TextureSubresourceSet &subresources =
                TextureSubresourceSet{ 0, 1, 0, TextureSubresourceSet::s_AllSlices },
            TextureDimension dimension = TextureDimension::Unknown )
        {
            BindingSetElement result;
            result.Type = ResourceType::Texture_UAV;
            result.Texture = texture;
            result.Slot = slot;
            result.BindingFormat = format;
            result.Dimension = dimension;
            result.Subresources = subresources;
            return result;
        }

        static BindingSetElement CreateTypedBufferSRV( Uint32 slot,
            GPUBufferHandle buffer,
            Format format = Format::UNKNOWN,
            BufferRange range = s_EntireBuffer )
        {
            BindingSetElement result;
            result.Type = ResourceType::TypedBuffer_SRV;
            result.Buffer = buffer;
            result.Slot = slot;
            result.BindingFormat = format;
            result.Range = range;
            return result;
        }

        static BindingSetElement CreateTypedBufferUAV( Uint32 slot,
            GPUBufferHandle buffer,
            Format format = Format::UNKNOWN,
            BufferRange range = s_EntireBuffer )
        {
            BindingSetElement result;
            result.Type = ResourceType::TypedBuffer_UAV;
            result.Buffer = buffer;
            result.Slot = slot;
            result.BindingFormat = format;
            result.Range = range;
            return result;
        }

        static BindingSetElement
        CreateConstantBuffer( Uint32 slot, GPUBufferHandle buffer, BufferRange range = s_EntireBuffer )
        {
            BindingSetElement result;
            result.Type = ResourceType::ConstantBuffer;
            result.Buffer = buffer;
            result.Slot = slot;
            result.BindingFormat = Format::UNKNOWN;
            result.Range = range;
            return result;
        }

        static BindingSetElement CreateSampler( Uint32 slot, SamplerHandle sampler )
        {
            BindingSetElement result;
            result.Type = ResourceType::Sampler;
            result.Sampler = sampler;
            result.Slot = slot;
            result.BindingFormat = Format::UNKNOWN;
            result.RawData[0] = 0;
            result.RawData[1] = 0;
            return result;
        }
    };

    struct BindingSetDescriptor final
    {
        BindingSetDescriptor( const std::initializer_list< BindingSetElement > &elements ) : Elements{ elements }
        {
        }

        void AddItem( const BindingSetElement &elem )
        {
            Elements.push_back( elem );
        }

        auto begin()
        {
            return Elements.begin();
        }

        auto begin() const
        {
            return Elements.begin();
        }

        auto end()
        {
            return Elements.end();
        }

        auto end() const
        {
            return Elements.end();
        }

        std::vector< BindingSetElement > Elements;
    };
}