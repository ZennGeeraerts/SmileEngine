/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

// Header file to include in Smile Engine applications

#include "smile_engine/common/compiled/compiled.h"
#include "smile_engine/common/logger/logger.h"
#include "smile_engine/core/application/application.h"
#include "smile_engine/core/application/layer.h"

#include "smile_engine/common/primitive/timestep.h"

#include "smile_engine/common/primitive/utils.h"
#include "smile_engine/core/math/math.h"

#include "smile_engine/core/input/input.h"
#include "smile_engine/core/input/key_codes.h"
#include "smile_engine/core/input/mouse_codes.h"
#include "smile_engine/core/input/gamepad_codes.h"

#include "smile_engine/graphic/imgui/imgui_layer.h"

#include "smile_engine/core/scene/scene.h"
#include "smile_engine/core/scene/entity.h"
#include "smile_engine/core/scene/components.h"

#include "smile_engine/core/project/project_manager.h"

/* ---Graphic--- */
//#include "smile_engine/graphic/render_engine.h"
//#include "smile_engine/graphic/render_command.h"
//
//#include "smile_engine/graphic/shader/shader.h"
//#include "smile_engine/graphic/resource/vertex_buffer.h"
//#include "smile_engine/graphic/resource/index_buffer.h"
//#include "smile_engine/graphic/resource/frame_buffer.h"
//#include "smile_engine/graphic/resource/texture.h"
#include "smile_engine/graphic/mesh/mesh_filter.h"
#include "smile_engine/graphic/mesh/skinned_mesh_filter.h"
#include "smile_engine/graphic/mesh/material.h"

/* --- DirectX --- */
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>