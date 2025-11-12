/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/text/string_view.h"

namespace smile::commandline
{
    class Arguments;

    template < typename ValueType >
    class Parser final
    {
      public:
        Parser() = default;
        ~Parser() = default;

        bool Parse( primitive::StringView optionName, ValueType &value, Arguments &args ) const;
    };
}