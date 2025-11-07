/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "run.h"

#include "option.h"
#include "option_map.h"
#include "arguments.h"

namespace smile::commandline
{
    bool Parse( primitive::ArrayView< const char * > args )
    {
        auto &optionMap = OptionBase::GetOptionMap();
        auto &options = optionMap.Options;
        auto &shortOptions = optionMap.ShortOptions;

        Arguments arguments{ args };

        while ( arguments.HasArgument() )
        {
            const Arg arg = arguments.Pop();
            const auto argName = arg.GetName();

            if ( !arg.IsShortOption() )
            {
                auto option = options.FindItemAtKey( argName );

                if ( option == options.end() )
                {
                    return false;
                }

                if ( !option.GetItem()->Parse( arguments ) )
                {
                    return false;
                }
            }
            else
            {
                auto shortOption = shortOptions.FindItemAtKey( argName[0] );

                if ( shortOption == shortOptions.end() )
                {
                    return false;
                }

                if ( !shortOption.GetItem()->Parse( arguments ) )
                {
                    return false;
                }
            }
        }

        return true;
    }
}