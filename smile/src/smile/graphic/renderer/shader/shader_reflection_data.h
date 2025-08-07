/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "smile/common/primitive/text/string.h"

#include "shader_blob_format.h"
#include "constant_buffer_descriptor.h"
#include "shader_resource_binding.h"
#include "smile/graphic/rhi/resource/buffer.h"

namespace smile::graphic
{
    struct ShaderReflectionData final
    {
        primitive::String EntryPoint;
        primitive::String TargetProfile;
        ShaderBlobFormat BlobFormat;
        rhi::BufferLayout InputSignature;
        rhi::BufferLayout OutputSignature;
        primitive::HashMap< primitive::String, ConstantBufferDescriptor > ConstantBufferDescs;
        primitive::Vector< ShaderResourceBinding > ShaderResourceBindings;
    };
}