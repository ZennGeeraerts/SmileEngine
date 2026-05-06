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
 * @file        skeleton.h
 * @author      Zenn Geeraerts
 * @created     6 May 2026
 * @brief       Holds the skeleton bone data
 */
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "smile/common/primitive/text/string.h"
#include "bone_info.h"

namespace smile::graphic
{
    class Skeleton final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Skeleton >;
        using ConstRef = memory::Ref< const Skeleton >;

        Skeleton() = default;

        const BoneInfo &GetBone( const primitive::StringView name ) const noexcept
        {
            return m_Bones[name];
        }

        BoneInfo &GetBone( const primitive::StringView name ) noexcept
        {
            return m_Bones[name];
        }

        auto FindBone( const primitive::StringView name ) const noexcept
        {
            return m_Bones.FindItemAtKey( name );
        }

        bool HasBone( const primitive::StringView name ) const noexcept
        {
            return m_Bones.HasItemAtKey( name );
        }

        Count GetBoneCount() const noexcept
        {
            return m_BoneCount;
        }

        auto begin() const noexcept
        {
            return m_Bones.begin();
        }

        auto end() const noexcept
        {
            return m_Bones.end();
        }

      private:
        primitive::HashMap< primitive::String, BoneInfo > m_Bones;
        Count m_BoneCount{ 0 };

        friend class ModelLoader;
    };
}