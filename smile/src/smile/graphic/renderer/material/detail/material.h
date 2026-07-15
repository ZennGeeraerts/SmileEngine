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
 * @file        material.h
 * @author      Zenn Geeraerts
 * @created     9 Januari 2026
 * @brief       Holds the data of a material and its default instance
 */
#pragma once

#include "material_handle.h"
#include "smile/graphic/renderer/material/material_layout.h"

namespace smile::graphic::detail
{
    class Material final
    {
      public:
        Material() = default;

        Material( const primitive::String &name,
            const MaterialLayout &layout,
            const MaterialInstanceHandle defaultInstanceHandle ) noexcept
            : m_Name{ name }, m_Layout{ layout }, m_DefaultInstanceHandle{ defaultInstanceHandle }
        {
        }

        ~Material() = default;

        primitive::StringView GetName() const noexcept
        {
            return m_Name.AsStringView();
        }

        const MaterialLayout &GetLayout() const noexcept
        {
            return m_Layout;
        }

        MaterialInstanceHandle GetDefaultInstance() const noexcept
        {
            return m_DefaultInstanceHandle;
        }

      private:
        primitive::String m_Name;
        MaterialLayout m_Layout;
        MaterialInstanceHandle m_DefaultInstanceHandle;
    };
}