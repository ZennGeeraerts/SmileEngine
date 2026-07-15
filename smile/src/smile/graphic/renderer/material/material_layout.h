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
 * @file        material_layout.h
 * @author      Zenn Geeraerts
 * @created     13 April 2026
 * @brief       The layout of a material
 */
#pragma once

#include "smile/common/foundation/flags.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/rhi/shader/shader_stage.h"
#include "smile/graphic/rhi/render_state.h"

namespace smile::graphic
{
    enum class MaterialParameterType
    {
        Unknown,
        Float,
        Int,
        Uint,
        Bool,
        Float2,
        Float3,
    };

    struct MaterialLayout final
    {
        struct Parameter final
        {
            Parameter() = default;

            Parameter( const primitive::String &name, MaterialParameterType type, Uint32 offset, Count size ) noexcept
                : Name{ name }, Type{ type }, Offset{ offset }, Size{ size }
            {
            }

            primitive::String Name;
            MaterialParameterType Type = MaterialParameterType::Unknown;
            Uint32 Offset = 0;
            Count Size = 0;

            inline bool operator==( const Parameter &other ) const noexcept
            {
                return Name == other.Name && Type == other.Type && Offset == other.Offset && Size == other.Size;
            }
        };

        struct Texture final
        {
            primitive::String Name;
            Uint32 Slot;

            inline bool operator==( const Texture &other ) const noexcept
            {
                return Name == other.Name && Slot == other.Slot;
            }
        };

        primitive::Vector< Parameter > Parameters;
        primitive::Vector< Texture > Textures;
        Uint32 CbSlot;
        Count CbSize;
        foundation::Flags< rhi::ShaderStage > Visibility;

        rhi::RenderState RenderState;
    };
}