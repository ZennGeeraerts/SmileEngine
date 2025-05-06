/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "shader_blob_format.h"
#include "constant_buffer_descriptor.h"
#include "shader_resource_binding.h"

#include <string>
#include <vector>

namespace smile::graphic
{
    struct ShaderReflectionData final
    {
        std::string EntryPoint;
        std::string TargetProfile;
        ShaderBlobFormat BlobFormat;
        BufferLayout InputSignature;
        BufferLayout OutputSignature;
        std::vector< ConstantBufferDescriptor > ConstantBufferDescs;
        std::vector< ShaderResourceBinding > ShaderResourceBindings;
    };
}