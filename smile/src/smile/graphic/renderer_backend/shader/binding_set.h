/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "resource_type.h"
#include "smile/graphic/renderer_backend/render_handle.h"

namespace smile::graphic
{
    struct BindingSetElement final
    {
        BindingSetElement() {};

        ResourceType Type;
        union
        {
            TextureHandle Texture;
            GPUBufferHandle Buffer;
            SamplerHandle Sampler;
        };
        Uint32 Slot;

        Format BindingFormat = Format::UNKNOWN; // Valid for textures, buffer srv and buffer uav

        union
        {
            BufferRange Range; // Valid for buffers
            std::array< Uint32, 2 > RawData;
        };

        static BindingSetElement Unknown( Uint32 slot )
        {
            BindingSetElement result;
            result.Type = ResourceType::Unknown;
            result.Slot = slot;
            result.BindingFormat = Format::UNKNOWN;
            return result;
        }

        static BindingSetElement Texture_SRV( Uint32 slot, TextureHandle texture, Format format = Format::UNKNOWN )
        {
            BindingSetElement result;
            result.Type = ResourceType::Texture_SRV;
            result.Texture = texture;
            result.Slot = slot;
            result.BindingFormat = format;
            return result;
        }

        static BindingSetElement Texture_UAV( Uint32 slot, TextureHandle texture, Format format = Format::UNKNOWN )
        {
            BindingSetElement result;
            result.Type = ResourceType::Texture_UAV;
            result.Texture = texture;
            result.Slot = slot;
            result.BindingFormat = format;
            return result;
        }

        static BindingSetElement TypedBuffer_SRV( Uint32 slot,
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

        static BindingSetElement TypedBuffer_UAV( Uint32 slot,
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
        ConstantBuffer( Uint32 slot, GPUBufferHandle buffer, BufferRange range = s_EntireBuffer )
        {
            BindingSetElement result;
            result.Type = ResourceType::ConstantBuffer;
            result.Buffer = buffer;
            result.Slot = slot;
            result.BindingFormat = Format::UNKNOWN;
            result.Range = range;
            return result;
        }

        static BindingSetElement Sampler( Uint32 slot, SamplerHandle sampler )
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

        std::vector< BindingSetElement > Elements;
    };
}