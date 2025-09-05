/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/world/ecs/transform_component.h"

#include "smile/graphic/scene/ecs/mesh_renderer_component.h"
#include "smile/graphic/scene/ecs/skinned_mesh_renderer_component.h"
#include "smile/graphic/scene/ecs/camera_component.h"
#include "smile/graphic/sprite/ecs/sprite_renderer_component.h"
#include "smile/graphic/animation/ecs/animator_component.h"

#include "smile/scripting/ecs/script_component.h"

#include "smile/physics/ecs/rigidbody_component.h"
#include "smile/physics/ecs/box_collider_component.h"
#include "smile/physics/ecs/sphere_collider_component.h"
#include "smile/physics/ecs/capsule_collider_component.h"
#include "smile/physics/ecs/character_controller_component.h"

namespace smile::scripting
{
    template < typename... Component >
    struct ComponentGroup
    {
    };

    using AllComponents = ComponentGroup< world::ecs::TransformComponent,
        graphic::ecs::MeshRendererComponent,
        graphic::ecs::SkinnedMeshRendererComponent,
        graphic::ecs::SpriteRendererComponent,
        graphic::ecs::AnimatorComponent,
        graphic::ecs::CameraComponent,
        scripting::ecs::ScriptComponent,
        physics::ecs::RigidbodyComponent,
        physics::ecs::BoxColliderComponent,
        physics::ecs::SphereColliderComponent,
        physics::ecs::CapsuleColliderComponent,
        physics::ecs::CharacterControllerComponent >;
}