/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "ecs/transform_component.h"

#include "engine/graphic/mesh/ecs/mesh_renderer_component.h"
#include "engine/graphic/mesh/ecs/skinned_mesh_renderer_component.h"
#include "engine/graphic/sprite/ecs/sprite_renderer_component.h"
#include "engine/graphic/animation/ecs/animator_component.h"
#include "engine/graphic/camera/ecs/camera_component.h"

#include "engine/scripting/ecs/script_component.h"

#include "engine/physics/ecs/rigidbody_component.h"
#include "engine/physics/ecs/box_collider_component.h"
#include "engine/physics/ecs/sphere_collider_component.h"
#include "engine/physics/ecs/capsule_collider_component.h"
#include "engine/physics/ecs/character_controller_component.h"

namespace smile::world
{
    template < typename... Component >
    struct ComponentGroup
    {
    };

    using AllComponents = ComponentGroup< ecs::TransformComponent,
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