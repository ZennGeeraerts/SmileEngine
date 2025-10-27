/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/array_view.h"
#include "smile/common/primitive/text/string_view.h"

namespace smile::commandline
{
    class ArgName final
    {
      public:
        ArgName( const primitive::StringView name ) : Name{ name }
        {
        }

        bool IsValid() const
        {
            return Name.GetCharCount() > 2 && Name[0] == '-' && Name[1] == '-';
        }

        primitive::StringView GetView() const
        {
            return { Name.GetData() + 2 };
        }

      private:
        primitive::StringView Name;
    };

    class ArgValue final
    {
      public:
        ArgValue( const primitive::StringView value ) : Value{ value }
        {
        }

        bool IsValid() const
        {
            return Value.GetCharCount() > 0 && Value[0] != '-';
        }

        primitive::StringView GetView() const
        {
            return Value;
        }

      private:
        primitive::StringView Value;
    };

    class Arg final
    {
      public:
        Arg( const primitive::StringView name, const primitive::StringView value ) : Name{ name }, Value{ value }
        {
        }

        bool IsValid() const
        {
            return Name.IsValid() && Value.IsValid();
        }

        primitive::StringView GetName() const
        {
            return Name.GetView();
        }

        primitive::StringView GetValue() const
        {
            return Value.GetView();
        }

      private:
        ArgName Name;
        ArgValue Value;
    };

    class Arguments final
    {
      public:
        explicit Arguments( primitive::ArrayView< const char * > args );
        ~Arguments() = default;

        bool HasArgument() const;

        Arg Pop();

        bool IsFinished() const;

      private:
        primitive::ArrayView< const char * > m_Arguments;
        Index m_CurrentIndex{ 0 };
    };
}