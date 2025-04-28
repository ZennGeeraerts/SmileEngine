/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "shader_blob_format.h"
#include "shader_io_signature.h"
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
        ShaderIOSignature InputSignature;
        ShaderIOSignature OutputSignature;
        std::vector< ConstantBufferDescriptor > ConstantBufferDescs;
        std::vector< ShaderResourceBinding > ShaderResourceBindings;
    };
}