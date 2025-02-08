/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "component_interface.h"

#include "ecs_engine.h"

namespace smile::ecs
{
    ComponentInterface::ComponentInterface( const ECSEngine &ecsEngine )
        : m_ECSEngine{ ecsEngine }
    {
    }

    ComponentInterface::~ComponentInterface()
    {
        SAFE_DELETE( m_pComponentStorage );
    }

    EntityHandleType ComponentInterface::GetEntityHandle( IndexType index ) const
    {
        auto entityIndex = m_Pool.GetElement( index );
        return m_ECSEngine.GetEntityHandleManager().GetEntityHandle( entityIndex );
    }
}