/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "group_iterator.h"

namespace smile::ecs
{
    class GroupBase
    {
      public:
        virtual void AddEntity( IndexType entityIndex ) = 0;
        virtual void RemoveEntity( IndexType entityIndex ) = 0;
        virtual bool HasComponent( ComponentInterface *pComponent ) const = 0;
        virtual bool HasEntity( IndexType entityIndex ) const = 0;

        virtual GroupIterator begin() const = 0;
        virtual GroupIterator end() const = 0;

        virtual const std::vector< ComponentInterface * > &GetOwnedComponents() const = 0;
        virtual const std::vector< ComponentInterface * > &GetGetComponents() const = 0;
    };
}