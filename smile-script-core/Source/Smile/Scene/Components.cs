/*=============================================================================*/
// Copyright 2022-2023 Smile Script Core
// Authors: Zenn Geeraerts
/*=============================================================================*/
namespace Smile
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
            }
        }
    }

    public class RigidbodyComponent : Component
    {
        public void AddForce(Vector3 force, bool autoAwake = true)
        {
            InternalCalls.RigidbodyComponent_AddForce(Entity.ID, ref force, autoAwake);
        }
    }

    public class CharacterControllerComponent : Component
    {
        public void Move(Vector3 displacement, float minDist = 0.0f)
        {
            InternalCalls.CharacterControllerComponent_Move(Entity.ID, ref displacement, minDist);
        }
    }
}
