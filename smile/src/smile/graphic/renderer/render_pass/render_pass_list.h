/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/type_id.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "render_pass.h"

namespace smile::graphic
{
    template < typename Type >
    concept RenderPassType = std::derived_from< Type, RenderPass > && requires {
        typename Type::Ref;
        typename Type::ConstRef;
    };

    class RenderPassList final
    {
      public:
        RenderPassList() = default;
        ~RenderPassList() noexcept;

        RenderPassList( const RenderPassList & ) = delete;
        RenderPassList( RenderPassList && ) = delete;
        RenderPassList &operator=( const RenderPassList & ) = delete;
        RenderPassList &operator=( RenderPassList && ) = delete;

        void Execute( const Framebuffer &framebuffer, const View &view ) noexcept;

        template < RenderPassType PassType, typename... Args >
        auto PushBack( Args &&...args ) -> typename PassType::Ref
        {
            auto typeID = foundation::TypeIDOf< PassType >();

            if ( auto it = m_RenderPassMap.FindItemAtKey( typeID ); it != m_RenderPassMap.end() )
            {
                m_RenderPasses.Erase( it.GetItem() );
            }

            auto pass = memory::CreateRef< PassType >( std::forward< Args >( args )... );
            pass->Initialize();

            m_RenderPasses.PushBack( pass );
            m_RenderPassMap[typeID] = pass;

            return pass;
        }

        template < RenderPassType PassType, typename... Args >
        auto PushFront( Args &&...args ) -> typename PassType::Ref
        {
            auto typeID = foundation::TypeIDOf< PassType >();

            if ( auto it = m_RenderPassMap.FindItemAtKey( typeID ); it != m_RenderPassMap.end() )
            {
                m_RenderPasses.Erase( it.GetItem() );
            }

            auto pass = memory::CreateRef< PassType >( std::forward< Args >( args )... );
            pass->Initialize();

            m_RenderPasses.PushFront( pass );
            m_RenderPassMap[typeID] = pass;

            return pass;
        }

        template < RenderPassType PassType >
        bool Remove() noexcept
        {
            auto typeID = foundation::TypeIDOf< PassType >();

            auto it = m_RenderPassMap.FindItemAtKey( typeID );
            if ( it == m_RenderPassMap.end() )
                return false;

            auto pass = it.GetItem();
            pass->ShutDown();

            m_RenderPasses.Erase( pass );
            m_RenderPassMap.Erase( it );

            return true;
        }

        template < RenderPassType PassType >
        [[nodiscard]] const PassType &Get() const
        {
            auto typeID = foundation::TypeIDOf< PassType >();
            return *static_cast< const PassType * >( m_RenderPassMap[typeID].GetPointer() );
        }

        template < RenderPassType PassType >
        [[nodiscard]] PassType &Get()
        {
            auto typeID = foundation::TypeIDOf< PassType >();
            return *static_cast< PassType * >( m_RenderPassMap[typeID].GetPointer() );
        }

        void Clear() noexcept;

        auto begin() const
        {
            return m_RenderPasses.begin();
        }

        auto end() const
        {
            return m_RenderPasses.end();
        }

      private:
        primitive::Vector< RenderPass::Ref > m_RenderPasses{};
        primitive::HashMap< foundation::TypeID, RenderPass::Ref > m_RenderPassMap{};
    };
}