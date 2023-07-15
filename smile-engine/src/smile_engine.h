/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

// Header file to include in Smile Engine applications

#include "smile_engine/core/core.h"
#include "smile_engine/core/application.h"
#include "smile_engine/core/layer.h"
#include "smile_engine/core/logger.h"

#include "smile_engine/core/timestep.h"

#include "smile_engine/utils/utils.h"
#include "smile_engine/math/math.h"

#include "smile_engine/input/input.h"
#include "smile_engine/input/key_codes.h"
#include "smile_engine/input/mouse_codes.h"
#include "smile_engine/input/gamepad_codes.h"

#include "smile_engine/imgui/imgui_layer.h"

#include "smile_engine/scene/scene.h"
#include "smile_engine/scene/entity.h"
#include "smile_engine/scene/components.h"

#include "smile_engine/project/project.h"

/* ---Renderer--- */
#include "smile_engine/graphic/render_engine.h"
#include "smile_engine/graphic/render_command.h"

#include "smile_engine/graphic/shader/shader.h"
#include "smile_engine/graphic/resource/vertex_buffer.h"
#include "smile_engine/graphic/resource/index_buffer.h"
#include "smile_engine/graphic/resource/frame_buffer.h"
#include "smile_engine/graphic/resource/texture.h"
#include "smile_engine/graphic/mesh/static_mesh_filter.h"
#include "smile_engine/graphic/mesh/material.h"

/* --- DirectX --- */
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>