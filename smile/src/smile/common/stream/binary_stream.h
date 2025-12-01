/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"
#include "opening_mode.h"
#include "smile/common/primitive/text/string_view.h"

namespace smile::stream
{
    class BinaryStream : public memory::Counted
    {
      public:
        BinaryStream();
        BinaryStream( const BinaryStream & ) = delete;
        BinaryStream( BinaryStream && ) = delete;
        virtual ~BinaryStream();

        BinaryStream &operator=( const BinaryStream & ) = delete;
        BinaryStream &operator=( BinaryStream && ) = delete;

        Count GetSize() const
        {
            SM_ASSERT_MSG( IsOpen(), "Binary stream is not open" );
            return m_Size;
        }

        Index GetIndex() const
        {
            SM_ASSERT_MSG( IsOpen(), "Binary stream is not open" );
            return m_Index;
        }

        bool IsAtEnd() const
        {
            return m_Index == m_Size;
        }

        constexpr bool IsOpen() const
        {
            return m_IsOpen;
        }

        constexpr bool IsInput() const
        {
            return m_IsInput;
        }

        constexpr bool IsOutput() const
        {
            return m_IsOutput;
        }

        virtual bool SetIndex( const Index index ) = 0;
        virtual bool OpenInput() = 0;
        virtual bool OpenOutput( OpeningModeFlags openingModeFlags ) = 0;
        virtual bool OpenInputOutput( OpeningModeFlags openingModeFlags ) = 0;

        virtual Count ReadByteArray( void *pByteArray, Count size ) = 0;
        virtual Count WriteByteArray( const void *pByteArray, const Count size ) = 0;

        virtual bool Close() = 0;

        template < typename Type, typename = std::void_t< std::is_arithmetic< Type > > >
        void ReadScalar( Type &value )
        {
            ReadByteArray( &value, sizeof( value ) );
        }

        template < typename Type, typename = std::void_t< std::is_arithmetic< Type > > >
        void WriteScalar( Type value )
        {
            WriteByteArray( &value, sizeof( value ) );
        }

        void WriteText( primitive::StringView text )
        {
            WriteByteArray( text.GetData(), text.GetCharCount() );
        }

      protected:
        void SetOpen( bool isOpen )
        {
            m_IsOpen = isOpen;
        }

        void SetInput( bool isInput )
        {
            m_IsInput = isInput;
        }

        void SetOutput( bool isOutput )
        {
            m_IsOutput = isOutput;
        }

        Count GetInternalSize() const
        {
            return m_Size;
        }

        void UpdateSize( const Count size )
        {
            m_Size = size;
        }

        void UpdateIndex( const Index index )
        {
            SM_ASSERT_MSG( index <= m_Size, "Index has to be smaller than size" );
            m_Index = index;
        }

      private:
        Count m_Size;
        Index m_Index;
        Uint32 m_IsOpen : 1, m_IsInput : 1, m_IsOutput : 1;
    };

    void GetTextContent( primitive::String &content, BinaryStream &stream );
}