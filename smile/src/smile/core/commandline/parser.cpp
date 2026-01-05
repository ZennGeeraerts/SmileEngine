/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "parser.h"

#include "smile/common/primitive/text/utils.h"
#include "arguments.h"

namespace smile::commandline
{
    template <>
    bool Parser< bool >::Parse( primitive::StringView optionName, bool &value, const Arg &arg ) const
    {
        const auto textValue = arg.GetValue();

        if ( textValue == "true" || textValue == "1" )
        {
            value = true;
        }
        else if ( textValue == "false" || textValue == "0" )
        {
            value = false;
        }
        else
        {
            SM_LOG_ERROR( "Failed to parse: '{0}' with value: '{1}': not a boolean", optionName, textValue );
            SM_LOG_ERROR( "Valid values are true, false, 1, 0" );

            return false;
        }

        return true;
    }

    template <>
    bool Parser< int >::Parse( primitive::StringView optionName, int &value, const Arg &arg ) const
    {
        const auto textValue = arg.GetValue();

        std::optional< int > intValue = primitive::ToInt( textValue );

        if ( intValue.has_value() )
        {
            value = intValue.value();
        }
        else
        {
            SM_LOG_ERROR( "Failed to parse: '{0}' with value: '{1}': not an integer", optionName, textValue );
            SM_LOG_ERROR( "Valid values are integers" );

            return false;
        }

        return true;
    }

    template <>
    bool Parser< float >::Parse( primitive::StringView optionName, float &value, const Arg &arg ) const
    {
        const auto textValue = arg.GetValue();

        std::optional< float > floatValue = primitive::ToFloat( textValue );

        if ( floatValue.has_value() )
        {
            value = floatValue.value();
        }
        else
        {
            SM_LOG_ERROR( "Failed to parse: '{0}' with value: '{1}': not a float", optionName, textValue );
            SM_LOG_ERROR( "Valid values are floats" );

            return false;
        }

        return true;
    }

    template <>
    bool Parser< primitive::String >::Parse( primitive::StringView optionName,
        primitive::String &value,
        const Arg &arg ) const
    {
        const auto textValue = arg.GetValue();

        if ( !textValue.IsEmpty() )
        {
            value = textValue;
        }
        else
        {
            SM_LOG_ERROR( "Failed to parse: '{0}' with value: '{1}': string is empty", optionName, textValue );
            SM_LOG_ERROR( "Valid values are strings" );

            return false;
        }

        return true;
    }
}