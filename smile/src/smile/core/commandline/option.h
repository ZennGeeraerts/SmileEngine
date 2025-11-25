/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "modifier.h"
#include "option_base.h"
#include "parser.h"

#include <filesystem>

namespace smile::commandline
{
    template < typename OptionType, typename Modifier >
    void ApplyModifier( OptionType &option, Modifier &&modifer )
    {
        std::forward< Modifier >( modifer ).Apply( option );
    }

    template < typename ValueType >
    struct OptionTraits final
    {
        using ParserType = Parser< ValueType >;
    };

    template <>
    struct OptionTraits< std::filesystem::path > final
    {
        using ParserType = Parser< primitive::String >;
    };

    template < typename ValueType >
    class Option final : public OptionBase
    {
      public:
        template < typename... Args >
        Option( const char *name, Args &&...arguments ) : OptionBase{ name }
        {
            [[maybe_unused]] int _[] = { ( ApplyModifier( *this, std::forward< Args >( arguments ) ), 0 )... };
            AppendOption();
        }

        operator const ValueType &() const
        {
            return Value;
        }

        const ValueType &operator*() const
        {
            return Value;
        }

        void SetValue( const ValueType &value )
        {
            Value = value;
        }

        bool Parse( Arguments &arguments ) override
        {
            using ParserType = typename OptionTraits< ValueType >::ParserType;

            const ParserType parser;
            return parser.Parse( Name, Value, arguments );
        }

        ValueType Value;
    };
}