/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "layer.h"

namespace smile
{
#ifdef SM_C_DEBUG
    Layer::Layer( const std::string &name ) : m_DebugName{ name }
    {
    }
#elif SM_C_RELEASE
    Layer::Layer( const std::string &name )
    {
    }
#endif

    Layer::~Layer()
    {
    }
}
