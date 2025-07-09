/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "constant_type.h"

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/text/string.h"

namespace smile::graphic
{
    struct ConstantBufferItem final
    {
        primitive::String Name;
        ConstantType Type;
        Count Size;
        Count ItemCount; // Used for arrays
    };

    class ConstantBufferDescriptor final
    {
      public:
        ConstantBufferDescriptor(
            std::initializer_list< std::tuple< primitive::String, ConstantType, Count, Count > > list );

        void Add( primitive::String name, ConstantType type, const Count size, const Count itemCount = 1 );

        Count GetSize() const
        {
            return m_Size;
        }

      private:
        primitive::Vector< ConstantBufferItem > m_Items;
        Count m_Size;
    };
}