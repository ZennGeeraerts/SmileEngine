/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        base64.cpp
 * @author      Zenn Geeraerts
 * @created     29 May 2026
 * @brief       Base64 encoding and decoding.
 */
#include "smpch.h"
#include "base64.h"

#include "smile/common/stream/char_stream.h"

namespace smile::primitive
{
    static const char s_PadChar = '=';
    static const char s_EncodingTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    void Base64::Encode( stream::CharStream &encoded, const primitive::Vector< Byte > &decoded )
    {
        const Count itemCount{ decoded.GetItemCount() };

        const Count tripletCount{ itemCount / 3 };
        const Count leftOver{ itemCount - ( tripletCount * 3 ) };

        for ( const Index index : foundation::GetCountIterator( tripletCount ) )
        {
            const Index tripletIndex = index * 3;

            const Byte firstByte = decoded[tripletIndex] >> 2;
            const Byte secondByte = ( ( decoded[tripletIndex] & 0x3 ) << 4 ) | ( decoded[tripletIndex + 1] >> 4 );
            const Byte thirdByte = ( ( decoded[tripletIndex + 1] & 0xf ) << 2 ) | ( decoded[tripletIndex + 2] >> 6 );
            const Byte fourthByte = ( decoded[tripletIndex + 2] & 0x3f );

            encoded << s_EncodingTable[firstByte] << s_EncodingTable[secondByte] << s_EncodingTable[thirdByte]
                    << s_EncodingTable[fourthByte];
        }

        switch ( leftOver )
        {
            case 0:
                break;

            case 1:
            {
                const Index tripletIndex = tripletCount * 3;
                const Byte firstByte = decoded[tripletIndex] >> 2;
                const Byte secondByte = ( decoded[tripletIndex] & 0x3 ) << 4;

                encoded << s_EncodingTable[firstByte] << s_EncodingTable[secondByte] << "==";
            }
            break;

            case 2:
            {
                const Index tripletIndex = tripletCount * 3;
                const Byte firstByte = decoded[tripletIndex] >> 2;
                const Byte secondByte = ( ( decoded[tripletIndex] & 0x3 ) << 4 ) | ( decoded[tripletIndex + 1] >> 4 );
                const Byte thirdByte = ( ( decoded[tripletIndex + 1] & 0xf ) << 2 );

                encoded << s_EncodingTable[firstByte] << s_EncodingTable[secondByte] << s_EncodingTable[thirdByte]
                        << "=";
            }
            break;
        }
    }

    bool Base64::Decode( primitive::Vector< Byte > &decoded, const primitive::StringView encoded )
    {
        SM_ASSERT( !( encoded.GetCharCount() % 4 ) );

        int padding{ 0 };

        const Count charCount = encoded.GetCharCount();

        if ( charCount > 2 )
        {
            if ( encoded[charCount - 1] == s_PadChar )
            {
                ++padding;
            }
            if ( encoded[charCount - 2] == s_PadChar )
            {
                ++padding;
            }
        }

        decoded.Reserve( ( ( charCount / 4 ) * 3 ) - padding );

        Uint32 temp{ 0 };

        for ( const auto quadIndex : foundation::GetCountIterator( charCount / 4 ) )
        {
            for ( Index i = 0; i < 4; ++i )
            {
                auto value = encoded[quadIndex * 4 + i];

                temp <<= 6;
                if ( value >= 'A' && value <= 'Z' )
                {
                    temp |= value - 'A';
                }
                else if ( value >= 'a' && value <= 'z' )
                {
                    temp |= value - 'a' + 26;
                }
                else if ( value >= '0' && value <= '9' )
                {
                    temp |= value - '0' + 52;
                }
                else if ( value == '+' )
                {
                    temp |= 62;
                }
                else if ( value == '/' )
                {
                    temp |= 63;
                }
                else if ( value == s_PadChar )
                {
                    SM_ASSERT( padding == 1 || padding == 2 );

                    switch ( padding )
                    {
                        case 1:
                            decoded.PushBack( ( temp >> 16 ) & 0xFF );
                            decoded.PushBack( ( temp >> 8 ) & 0xFF );
                            return true;
                        case 2:
                            decoded.PushBack( ( temp >> 10 ) & 0xFF );
                            return true;
                        default:
                            return false;
                    }
                }
                else
                {
                    return false;
                }
            }

            decoded.PushBack( ( temp >> 16 ) & 0xFF );
            decoded.PushBack( ( temp >> 8 ) & 0xFF );
            decoded.PushBack( ( temp ) & 0xFF );
        }

        return true;
    }
}