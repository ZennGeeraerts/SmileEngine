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
        ArgName( const primitive::StringView name ) : m_Name{ name }
        {
        }

        bool IsValid() const
        {
            return ( m_Name.GetCharCount() == 2 && m_Name[0] == '-' && std::isalpha( m_Name[1] ) ) ||
                   ( m_Name.GetCharCount() > 2 && m_Name[0] == '-' && m_Name[1] == '-' );
        }

        primitive::StringView GetView() const
        {
            if ( m_Name.GetCharCount() == 2 )
            {
                return { m_Name.GetData() + 1 };
            }
            else
            {
                return { m_Name.GetData() + 2 };
            }
        }

        operator bool() const
        {
            return m_Name.GetCharCount() > 0;
        }

      private:
        primitive::StringView m_Name;
    };

    class ArgValue final
    {
      public:
        ArgValue( const primitive::StringView value ) : m_Value{ value }
        {
        }

        bool IsValid() const
        {
            return m_Value.GetCharCount() > 0 && m_Value[0] != '-';
        }

        primitive::StringView GetView() const
        {
            return m_Value;
        }

      private:
        primitive::StringView m_Value;
    };

    class Arg final
    {
      public:
        Arg( const primitive::StringView name, const primitive::StringView value ) : m_Name{ name }, m_Value{ value }
        {
        }

        Arg( const primitive::StringView value ) : m_Name{ nullptr }, m_Value{ value }
        {
        }

        bool IsValid() const
        {
            return ( IsPositional() || m_Name.IsValid() ) && m_Value.IsValid();
        }

        primitive::StringView GetName() const
        {
            if ( !IsPositional() )
            {
                return m_Name.GetView();
            }
            else
            {
                return nullptr;
            }
        }

        primitive::StringView GetValue() const
        {
            return m_Value.GetView();
        }

        bool IsShortOption() const
        {
            return !IsPositional() && GetName().GetCharCount() == 1;
        }

        bool IsPositional() const
        {
            return !m_Name;
        }

      private:
        ArgName m_Name;
        ArgValue m_Value;
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