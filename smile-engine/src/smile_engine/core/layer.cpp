/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "layer.h"

namespace smile
{
    Layer::Layer( const std::string &name ) : m_DebugName{ name }
    {
    }

    Layer::~Layer()
    {
    }
}
