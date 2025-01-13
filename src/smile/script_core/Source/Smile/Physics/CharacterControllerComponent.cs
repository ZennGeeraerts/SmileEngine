/*=============================================================================*/
// Copyright 2022-2024 Smile Script Core
// Authors: Zenn Geeraerts
/*=============================================================================*/

namespace Smile
{
    public class CharacterControllerComponent : Component
    {
        public void Move(Vector3 displacement, float minDist = 0.0f)
        {
            InternalCalls.CharacterControllerComponent_Move(Entity.ID, ref displacement, minDist);
        }
    }
}