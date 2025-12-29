/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "utils.h"

namespace smile::primitive
{
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

    Index
    FindText( const primitive::StringView text, const primitive::StringView textToFind, const Index firstCharIndex )
    {
        if ( textToFind.GetCharCount() == 0 )
        {
            return s_InvalidIndex;
        }

        if ( text.GetCharCount() < textToFind.GetCharCount() )
        {
            return s_InvalidIndex;
        }

        const Count lastCharToSearch = text.GetCharCount() - textToFind.GetCharCount();
        for ( auto index = firstCharIndex; index <= lastCharToSearch; ++index )
        {
            if ( text[index] == textToFind[0] )
            {
                Index subindex{ 0 };
                for ( ; subindex < textToFind.GetCharCount(); ++subindex )
                {
                    if ( text[index + subindex] != textToFind[subindex] )
                    {
                        break;
                    }
                }

                if ( subindex == textToFind.GetCharCount() )
                {
                    return index;
                }
            }
        }

        return s_InvalidIndex;
    }

    Index FindCharacter( const primitive::StringView text, const char charToFind, const Index firstCharIndex )
    {
        for ( auto index = firstCharIndex, count = text.GetCharCount(); index < count; ++index )
        {
            if ( text[index] == charToFind )
            {
                return index;
            }
        }

        return s_InvalidIndex;
    }

    void ReplaceText( String &text, const StringView textToFind, const StringView textToReplace )
    {
        SM_ASSERT( !textToFind.IsEmpty() );

        Index index;
        Index nextSearchIndex{ 0 };

        while ( ( index = FindText( text, textToFind, nextSearchIndex ) ) != s_InvalidIndex )
        {
            ReplaceTextInsideRange( text, index, textToFind.GetCharCount(), textToReplace );
            nextSearchIndex = index + textToReplace.GetCharCount();
        }
    }

    void
    ReplaceTextInsideRange( String &text, const Index index, const Count charCount, const StringView textToReplace )
    {
        SM_ASSERT( text.IsValidRange( index, charCount ) );

        const auto newCharCount = textToReplace.GetCharCount();

        if ( charCount > newCharCount )
        {
            const auto offset{ charCount - newCharCount };
            for ( auto moveIndex = index + newCharCount; moveIndex < ( text.GetCharCount() - offset ); ++moveIndex )
            {
                text[moveIndex] = text[moveIndex + offset];
            }

            text.SetCharCount( text.GetCharCount() - offset );
        }
        else if ( charCount < newCharCount )
        {
            const auto offset{ newCharCount - charCount };

            const auto prevCharCount{ text.GetCharCount() };
            text.SetCharCount( prevCharCount + offset );

            for ( auto moveIndex = prevCharCount; moveIndex-- > index; )
            {
                text[moveIndex + offset] = text[moveIndex];
            }
        }

        for ( auto textIndex : foundation::GetCountIterator( textToReplace.GetCharCount() ) )
        {
            text[index + textIndex] = textToReplace[textIndex];
        }
    }

    void ToLowerCase( String &text )
    {
        for ( const auto index : foundation::GetCountIterator( text.GetCharCount() ) )
        {
            text[index] = static_cast< unsigned char >( foundation::NumericCast< char >( tolower( text[index] ) ) );
        }
    }

    void ToUpperCase( String &text )
    {
        for ( const auto index : foundation::GetCountIterator( text.GetCharCount() ) )
        {
            text[index] = foundation::NumericCast< char >( toupper( text[index] ) );
        }
    }
}