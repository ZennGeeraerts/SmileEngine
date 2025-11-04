/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "utils.h"

namespace smile::primitive
{
    std::string WideStringToString( const std::wstring &wideString )
    {
        std::string str{ wideString.begin(), wideString.end() };
        return str;
    }

    std::wstring StringToWideString( const std::string &str )
    {
        std::wstring wideString{ str.begin(), str.end() };
        return wideString;
    }

    std::optional< int > ToInt( StringView text )
    {
        if ( text.IsEmpty() )
        {
            return std::nullopt;
        }

        const bool isNegative{ text[0] == '-' };
        Index textIndex = [&]()
        {
            if ( text[0] == '+' || isNegative )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }();

        const Count charCount = text.GetCharCount();
        if ( textIndex >= charCount )
        {
            return std::nullopt;
        }

        int result{ 0 };

        while ( textIndex < charCount )
        {
            const auto character = text[textIndex];
            if ( character >= '0' && character <= '9' )
            {
                result = result * 10 + ( character - '0' );
            }
            else
            {
                SM_LOG_ERROR( "Invalid integer '{}' passed to ToInt", text );
                return std::nullopt;
            }

            ++textIndex;
        }

        return isNegative ? -result : result;
    }

    std::optional< float > ToFloat( StringView text )
    {
        if ( text.IsEmpty() )
        {
            return std::nullopt;
        }

        const bool isNegative{ text[0] == '-' };
        bool isDecimal{ text[0] == '.' };

        Count charactersPastDecimal{ 0 };
        Index textIndex = [&]()
        {
            if ( text[0] == '+' || isNegative )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }();

        if ( text[textIndex] == '.' )
        {
            isDecimal = true;
            ++charactersPastDecimal;
            ++textIndex;
        }

        const Count charCount = text.GetCharCount();
        if ( textIndex >= charCount )
        {
            return std::nullopt;
        }

        float result{ 0.0f };

        while ( textIndex < charCount )
        {
            const auto character = text[textIndex];

            if ( character >= '0' && character <= '9' )
            {
                if ( !isDecimal )
                {
                    result = result * 10 + ( character - '0' );
                }
                else
                {
                    result = result + ( character - '0' ) / pow( 10, charactersPastDecimal );
                    ++charactersPastDecimal;
                }
            }
            else if ( character == '.' )
            {
                isDecimal = true;
                ++charactersPastDecimal;
            }
            else
            {
                SM_LOG_ERROR( "Invalid float '{}' passed to ToFloat", text );
                return std::nullopt;
            }

            ++textIndex;
        }

        return isNegative ? -result : result;
    }
}