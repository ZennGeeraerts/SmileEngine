#pragma once

// Header file to include in Smile Engine applications

#include "SmileEngine/Core/Application.h"
#include "SmileEngine/Core/Layer.h"
#include "SmileEngine/Core/Logger.h"

#include "SmileEngine/Core/Timestep.h"

#include "SmileEngine/Utils/Utils.h"
#include "SmileEngine/Math/Math.h"
#include "SmileEngine/Math/Vector2.h"
#include "SmileEngine/Math/Vector3.h"
#include "SmileEngine/Math/Vector4.h"

#include "SmileEngine/Input/Input.h"
#include "SmileEngine/Input/KeyCodes.h"
#include "SmileEngine/Input/MouseCodes.h"
#include "SmileEngine/Input/GamepadCodes.h"

#include "SmileEngine/UI/ImGui/ImGuiLayer.h"

#include "SmileEngine/Scene/Scene.h"
#include "SmileEngine/Scene/Entity.h"
#include "SmileEngine/Scene/Components.h"

/* ---Renderer--- */
#include "SmileEngine/Renderer/Renderer.h"
#include "SmileEngine/Renderer/RenderCommand.h"

#include "SmileEngine/Renderer/Shader/Shader.h"
#include "SmileEngine/Renderer/Resource/VertexBuffer.h"
#include "SmileEngine/Renderer/Resource/IndexBuffer.h"
#include "SmileEngine/Renderer/Resource/Framebuffer.h"
#include "SmileEngine/Renderer/Resource/Texture.h"
#include "SmileEngine/Renderer/Mesh/StaticMeshFilter.h"
#include "SmileEngine/Renderer/Mesh/Material.h"

/* --- DirectX --- */
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>