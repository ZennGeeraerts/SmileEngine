/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::commandline
{
    class OptionBase;

    template < typename ValueType >
    class Option;

    template < typename ValueType >
    struct DefaultValue final
    {
        explicit DefaultValue( const ValueType &value ) noexcept : Value{ value }
        {
        }

        template < typename OtherValueType >
        void Apply( Option< OtherValueType > &initValue ) &&;

        const ValueType &Value;
    };

    struct Description final
    {
        explicit Description( const char *desc ) noexcept : Desc{ desc }
        {
        }

        void Apply( OptionBase &option );

        const char *Desc;
    };

    struct Short final
    {
        explicit Short( const char oneLetterOption ) noexcept : OneLetterOption{ oneLetterOption }
        {
        }

        void Apply( OptionBase &option );

        const char OneLetterOption;
    };
}

#include "option.h"

namespace smile::commandline
{
    template < typename ValueType >
    template < typename OtherValueType >
    void DefaultValue< ValueType >::Apply( Option< OtherValueType > &option ) &&
    {
        option.Value = std::move( Value );
    }
}