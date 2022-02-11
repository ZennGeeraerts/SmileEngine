#pragma once

// Header file to include in Smile Engine applications

#include "SmileEngine/Core/SmileGame.h"
#include "SmileEngine/Core/Layer.h"
#include "SmileEngine/Core/Logger.h"

#include "SmileEngine/Core/Timestep.h"

#include "SmileEngine/Core/Input.h"
#include "SmileEngine/Core/KeyCodes.h"

#include "SmileEngine/ImGui/ImGuiLayer.h"

#include "SmileEngine/Scene/Scene.h"
#include "SmileEngine/Scene/Entity.h"
#include "SmileEngine/Scene/Components.h"

#include "SmileEngine/Utils/Utils.h"

#include "SmileEngine/Math/Math.h"

/* ---Renderer--- */
#include "SmileEngine/Renderer/Renderer.h"
#include "SmileEngine/Renderer/RenderCommand.h"

#include "SmileEngine/Renderer/Buffer.h"
#include "SmileEngine/Renderer/Shader.h"
#include "SmileEngine/Renderer/Framebuffer.h"
#include "SmileEngine/Renderer/Texture.h"
#include "SmileEngine/Renderer/StaticMeshFilter.h"
#include "SmileEngine/Renderer/Material.h"
#include "SmileEngine/Renderer/VertexArray.h"

#include "SmileEngine/Renderer/OrthographicCamera.h"
#include "SmileEngine/Renderer/PerspectiveCamera.h"
#include "SmileEngine/Renderer/PerspectiveCameraController.h"

/* --- DirectX --- */
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>