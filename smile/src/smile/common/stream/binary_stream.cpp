/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "binary_stream.h"

namespace smile::stream
{
    BinaryStream::~BinaryStream()
    {
        SM_ASSERT_MSG( !IsOpen(), "Binary stream is open while being destructed" );
    }
}