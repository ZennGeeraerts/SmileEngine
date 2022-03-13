#pragma once

// Header file to include in Smile Engine applications

#include "smile_engine/core/application.h"
#include "smile_engine/core/layer.h"
#include "smile_engine/core/logger.h"

#include "smile_engine/core/timestep.h"

#include "smile_engine/utils/utils.h"
#include "smile_engine/math/math.h"
#include "smile_engine/math/vector2.h"
#include "smile_engine/math/vector3.h"
#include "smile_engine/math/vector4.h"

#include "smile_engine/input/input.h"
#include "smile_engine/input/key_codes.h"
#include "smile_engine/input/mouse_codes.h"
#include "smile_engine/input/gamepad_codes.h"

#include "smile_engine/ui/imgui/imgui_layer.h"

#include "smile_engine/scene/scene.h"
#include "smile_engine/scene/entity.h"
#include "smile_engine/scene/components.h"

/* ---Renderer--- */
#include "smile_engine/renderer/renderer.h"
#include "smile_engine/renderer/render_command.h"

#include "smile_engine/renderer/shader/shader.h"
#include "smile_engine/renderer/resource/vertex_buffer.h"
#include "smile_engine/renderer/resource/index_buffer.h"
#include "smile_engine/renderer/resource/frame_buffer.h"
#include "smile_engine/renderer/resource/texture.h"
#include "smile_engine/renderer/mesh/static_mesh_filter.h"
#include "smile_engine/renderer/mesh/material.h"

/* --- DirectX --- */
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>