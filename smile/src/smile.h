/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

// Header file to include in Smile Engine applications

#include "smile/common/foundation/compiled.h"
#include "smile/common/logging/logging.h"
#include "smile/core/application/application.h"
#include "smile/core/application/layer.h"

#include "smile/common/primitive/timestep.h"

#include "smile/common/primitive/utils.h"
#include "smile/core/math/math.h"

#include "smile/core/input/input.h"
#include "smile/core/input/key_codes.h"
#include "smile/core/input/mouse_codes.h"
#include "smile/core/input/gamepad_codes.h"

#include "smile/graphic/imgui/imgui_layer.h"

#include "smile/core/world/world.h"
#include "smile/core/world/entity.h"
#include "smile/core/world/components.h"

#include "smile/core/project/project_manager.h"

/* ---Graphic--- */
//#include "smile/graphic/render_engine.h"
//#include "smile/graphic/render_command.h"
//
//#include "smile/graphic/shader/shader.h"
//#include "smile/graphic/resource/vertex_buffer.h"
//#include "smile/graphic/resource/index_buffer.h"
//#include "smile/graphic/resource/frame_buffer.h"
//#include "smile/graphic/resource/texture.h"
#include "smile/graphic/scene/mesh_filter.h"
#include "smile/graphic/scene/skinned_mesh_filter.h"
#include "smile/graphic/scene/material.h"

/* --- DirectX --- */
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>