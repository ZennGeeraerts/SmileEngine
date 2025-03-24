/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/uuid.h"
#include "smile/common/primitive/timestep.h"

#include "smile/core/ecs/ecs_engine.h"
#include "smile/core/ecs/state/state_manager.h"

#include "ecs/id_component.h"
#include "ecs/tag_component.h"

namespace smile::world
{
    class Entity;

    class World final
    {
      public:
        World();
        ~World();

        memory::Ref< smile::ecs::state::State > CreateState( const std::string &name );
        void ChangeState( const std::string &name );

        Entity CreateEntity();
        Entity CreateEntity( const std::string &name );
        Entity CreateEntity( primitive::UUID uuid, const std::string &name );
        void DestroyEntity( Entity entity );

        void OnOpen();
        void OnClose();

        void OnUpdate( primitive::Timestep deltaTime );

        void DuplicateEntity( Entity entity );

        Entity GetEntityByUUID( primitive::UUID uuid );

        primitive::UUID GetUUID() const
        {
            return m_UUID;
        }

        template < typename ComponentType >
        void OnComponentAdded()
        {
            RegisterCopyComponentFunctionsIfNeeded< ComponentType >();
        }

        template <>
        inline void OnComponentAdded< ecs::IDComponent >()
        {
            // We don't want to copy ID components, they have to be unique
        }

        template <>
        inline void OnComponentAdded< ecs::TagComponent >()
        {
            // We don't want to copy Tag components, they have to be unique
        }

      private:
        struct CopyComponentFunctions final
        {
            std::function< void( Entity, Entity ) > EntityCopy;
            std::function< void( smile::ecs::ECSEngine &,
                smile::ecs::ECSEngine &,
                const std::unordered_map< primitive::UUID, smile::ecs::EntityHandle > & ) >
                ECSEngineCopy;
        };

        // Register how components should be copied from one entity or ecs engine to another
        template < typename ComponentType >
        void RegisterCopyComponentFunctionsIfNeeded()
        {
            const auto typeID = foundation::TypeIDOf< ComponentType >();

            if ( s_CopyComponentFuncs.find( typeID ) != s_CopyComponentFuncs.end() )
                return;

            s_CopyComponentFuncs[typeID].EntityCopy = []( Entity src, Entity dst )
            {
                if ( src.HasComponent< ComponentType >() )
                    dst.AddOrReplaceComponent< ComponentType >( src.GetComponent< ComponentType >() );
            };

            s_CopyComponentFuncs[typeID].ECSEngineCopy =
                []( smile::ecs::ECSEngine &src,
                    smile::ecs::ECSEngine &dst,
                    const std::unordered_map< primitive::UUID, smile::ecs::EntityHandle > &entityMap )
            {
                auto view = src.GetView< ComponentType >();
                for ( auto entity : view )
                {
                    primitive::UUID uuid = src.GetComponent< ecs::IDComponent >( entity ).ID;
                    SM_ASSERT( entityMap.find( uuid ) != entityMap.end(), "UUID not found in entity map" );
                    smile::ecs::EntityHandle dstHandleID = entityMap.at( uuid );

                    auto &component = src.GetComponent< ComponentType >( entity );
                    dst.AddOrReplaceComponent< ComponentType >( dstHandleID, component );
                }
            };
        }

        static Ref< World > Copy( const Ref< World > &pWorld );

      private:
        primitive::UUID m_UUID;
        smile::ecs::ECSEngine m_ECSEngine;
        std::unordered_map< primitive::UUID, smile::ecs::EntityHandle > m_EntityMap{};
        smile::ecs::state::StateManager m_StateManager;

        static std::unordered_map< foundation::TypeID, CopyComponentFunctions > s_CopyComponentFuncs;

        friend class Entity;
        friend class WorldSerializer;
        friend class WorldHierarchyPanel;
        friend class WorldManager;
    };
}
