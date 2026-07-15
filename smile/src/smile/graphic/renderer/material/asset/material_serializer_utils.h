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
 * @file        material_serializer_utils.h
 * @author      Zenn Geeraerts
 * @created     5 May 2026
 * @brief       Utility functions for serializing material assets to a file
 */
#pragma once

#include "smile/core/yaml/yaml.h"
#include "smile/graphic/renderer/material/material_layout.h"
#include "material_asset_descriptor.h"

namespace smile::graphic
{
    void SerializeMaterialAssetDescriptor( const MaterialLayout &layout,
        const MaterialAssetDescriptor &desc,
        yaml::Emitter &output );

    void DeserializeMaterialAssetDescriptor( const yaml::Node &node,
        const MaterialLayout &layout,
        MaterialAssetDescriptor &desc );
}