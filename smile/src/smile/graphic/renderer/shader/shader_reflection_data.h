/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "shader_blob_format.h"
#include "shader_io_signature.h"

namespace smile::graphic
{
    class ShaderReflectionData final
    {
      public:
      private:
        std::string m_EntryPoint;
        std::string m_TargetProfile;
        ShaderBlobFormat m_BlobFormat;
        ShaderIOSignature m_InputSignature;
        ShaderIOSignature m_OutputSignature;
    };
}