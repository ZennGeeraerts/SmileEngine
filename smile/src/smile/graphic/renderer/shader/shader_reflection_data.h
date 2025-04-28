/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "shader_blob_format.h"
#include "shader_io_signature.h"
#include "constant_buffer_descriptor.h"
#include "shader_resource_descriptor.h"

#include <string>
#include <vector>

namespace smile::graphic
{
    struct ShaderReflectionData final
    {
        std::string m_EntryPoint;
        std::string m_TargetProfile;
        ShaderBlobFormat m_BlobFormat;
        ShaderIOSignature m_InputSignature;
        ShaderIOSignature m_OutputSignature;
        std::vector< ConstantBufferDescriptor > m_ConstantBufferDescs;
        std::vector< ShaderResourceDescriptor > m_ShaderResourceDescs;
    };
}