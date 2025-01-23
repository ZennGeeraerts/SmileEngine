/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

// Header file to include in Smile Engine applications

#include "engine/common/foundation/compiled.h"
#include "engine/common/logger/logger.h"
#include "engine/core/application/application.h"
#include "engine/core/application/layer.h"

#include "engine/common/primitive/timestep.h"

#include "engine/common/primitive/utils.h"
#include "engine/core/math/math.h"

#include "engine/core/input/input.h"
#include "engine/core/input/key_codes.h"
#include "engine/core/input/mouse_codes.h"
#include "engine/core/input/gamepad_codes.h"

#include "engine/graphic/imgui/imgui_layer.h"

#include "engine/core/scene/scene.h"
#include "engine/core/scene/entity.h"
#include "engine/core/scene/components.h"

#include "engine/core/project/project_manager.h"

/* ---Graphic--- */
//#include "engine/graphic/render_engine.h"
//#include "engine/graphic/render_command.h"
//
//#include "engine/graphic/shader/shader.h"
//#include "engine/graphic/resource/vertex_buffer.h"
//#include "engine/graphic/resource/index_buffer.h"
//#include "engine/graphic/resource/frame_buffer.h"
//#include "engine/graphic/resource/texture.h"
#include "engine/graphic/mesh/mesh_filter.h"
#include "engine/graphic/mesh/skinned_mesh_filter.h"
#include "engine/graphic/mesh/material.h"

/* --- DirectX --- */
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>