/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

// Header file to include in Smile Engine applications

#include "foundation/compiled.h"
#include "logging/logging.h"
#include "application/application.h"
#include "application/layer.h"

#include "primitive/timestep.h"

#include "primitive/utils.h"
#include "math/math.h"

#include "input/input.h"
#include "input/key_codes.h"
#include "input/mouse_codes.h"
#include "input/gamepad_codes.h"

#include "engine/graphic/imgui/imgui_layer.h"

#include "world/world.h"
#include "world/entity.h"
#include "world/components.h"

#include "project/project_manager.h"

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