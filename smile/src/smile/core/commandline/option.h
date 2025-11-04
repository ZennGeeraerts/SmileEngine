/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "option_base.h"
#include "parser.h"

namespace smile::commandline
{
    template < typename ValueType >
    class Option final : public OptionBase
    {
      public:
        template < typename... Args >
        Option( const char *name, Args &&...arguments ) : OptionBase{ name }
        {
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
            const Parser< ValueType > parser;
            parser.Parse( Name, Value, arguments );
        }

        ValueType Value;
    };
}