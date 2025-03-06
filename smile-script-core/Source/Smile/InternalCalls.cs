/*=============================================================================*/
// Copyright 2022-2023 Smile Script Core
// Authors: Zenn Geeraerts
/*=============================================================================*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Smile
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]

        internal extern static void TransformComponent_GetTranslation(ulong entityID, out Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(ulong entityID, ref Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidbodyComponent_AddForce(ulong entityID, ref Vector3 force, bool autoAwake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CharacterControllerComponent_Move(ulong entityID, ref Vector3 displacement, float minDist);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyPressed(KeyCode keyCode);
    }
}
