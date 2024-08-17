/*=============================================================================*/
// Copyright 2022-2024 Smile Script Core
// Authors: Zenn Geeraerts
/*=============================================================================*/

namespace Smile
{
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
}