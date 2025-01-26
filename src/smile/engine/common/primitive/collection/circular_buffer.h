/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::primitive
{
    template < typename Type >
    class CircularBuffer final
    {
      public:

      private:
        Type *m_pData;
        Uint32 m_ReadPos;
        Uint32 m_WritePos;
        Uint32 m_InUse;
        const Uint32 m_Capacity;
    };
}