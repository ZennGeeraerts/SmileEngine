/*=============================================================================*/
// Copyright 2022-2024 Smile Script Core
// Authors: Zenn Geeraerts
/*=============================================================================*/

namespace Smile
{
    public class RigidbodyComponent : Component
    {
        public void AddForce(Vector3 force, bool autoAwake = true)
        {
            InternalCalls.RigidbodyComponent_AddForce(Entity.ID, ref force, autoAwake);
        }
    }
}