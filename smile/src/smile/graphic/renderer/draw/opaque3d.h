/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        opaque3d.h
 * @author      Zenn Geeraerts
 * @created     2 July 2026
 * @brief       Opaque 3D draw command for the binned command buffer.
 */
#pragma once

#include "sort_key.h"
#include "smile/graphic/renderer/resource/graphics_pipeline.h"
#include "smile/graphic/renderer/material/material.h"
#include "smile/core/ecs/entity_handle.h"

namespace smile::graphic
{
    class Opaque3dBinKey final
    {
      public:
        Opaque3dBinKey() = default;

        constexpr Opaque3dBinKey( const Uint64 psoIndex, const Uint64 materialIndex, const Uint64 assetIndex ) noexcept
        {
            SM_ASSERT( psoIndex <= s_MaxFieldIndex );
            SM_ASSERT( materialIndex <= s_MaxFieldIndex );
            SM_ASSERT( assetIndex <= s_MaxFieldIndex );

            const Uint64 pso = psoIndex << 48;
            const Uint64 mat = materialIndex << 32;
            const Uint64 asset = assetIndex;
            m_Value = pso | mat | asset;
        }

        foundation::HashCode GetHashCode() const noexcept
        {
            return std::hash< Uint64 >{}( m_Value );
        }

        constexpr bool operator<( const Opaque3dBinKey other ) const noexcept
        {
            return m_Value < other.m_Value;
        }

        constexpr bool operator==( const Opaque3dBinKey other ) const noexcept
        {
            return m_Value == other.m_Value;
        }

        constexpr bool operator!=( const Opaque3dBinKey other ) const noexcept
        {
            return m_Value != other.m_Value;
        }

      private:
        static constexpr Uint64 s_MaxFieldIndex = 0xFFFFu;

        Uint64 m_Value{ 0 };
    };

    class Opaque3d final
    {
      public:
        using BinKey = Opaque3dBinKey;

        constexpr Opaque3d( smile::ecs::EntityHandle entity,
            const GraphicsPipeline &pipeline,
            const MaterialInstance &materialInstance,
            float depth ) noexcept
            : m_SortKey{ sort_key::EncodeOpaque( pipeline.GetHandle().GetIndex(),
                  materialInstance.GetHandle().GetIndex(),
                  depth ) },
              m_Entity{ entity }
        {
        }

        SortKey GetSortKey() const noexcept
        {
            return m_SortKey;
        }

        smile::ecs::EntityHandle GetEntity() const noexcept
        {
            return m_Entity;
        }

      private:
        SortKey m_SortKey;
        smile::ecs::EntityHandle m_Entity;
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::Opaque3dBinKey >
    {
        smile::foundation::HashCode operator()( const smile::graphic::Opaque3dBinKey key ) const noexcept
        {
            return key.GetHashCode();
        }
    };
}