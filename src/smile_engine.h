/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

// Header file to include in Smile Engine applications

#include "smile/engine/common/compiled/compiled.h"
#include "smile/engine/common/logger/logger.h"
#include "smile/engine/core/application/application.h"
#include "smile/engine/core/application/layer.h"

#include "smile/engine/common/primitive/timestep.h"

#include "smile/engine/common/primitive/utils.h"
#include "smile/engine/core/math/math.h"

#include "smile/engine/core/input/input.h"
#include "smile/engine/core/input/key_codes.h"
#include "smile/engine/core/input/mouse_codes.h"
#include "smile/engine/core/input/gamepad_codes.h"

#include "smile/engine/graphic/imgui/imgui_layer.h"

#include "smile/engine/core/scene/scene.h"
#include "smile/engine/core/scene/entity.h"
#include "smile/engine/core/scene/components.h"

#include "smile/engine/core/project/project_manager.h"

/* ---Graphic--- */
//#include "smile/engine/graphic/render_engine.h"
//#include "smile/engine/graphic/render_command.h"
//
//#include "smile/engine/graphic/shader/shader.h"
//#include "smile/engine/graphic/resource/vertex_buffer.h"
//#include "smile/engine/graphic/resource/index_buffer.h"
//#include "smile/engine/graphic/resource/frame_buffer.h"
//#include "smile/engine/graphic/resource/texture.h"
#include "smile/engine/graphic/mesh/mesh_filter.h"
#include "smile/engine/graphic/mesh/skinned_mesh_filter.h"
#include "smile/engine/graphic/mesh/material.h"

/* --- DirectX --- */
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>