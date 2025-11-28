/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "binary_stream.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"

namespace smile::stream
{
    class MemoryBinaryStream final : public BinaryStream
    {
      public:
        using Ref = memory::Ref< BinaryStream >;

        MemoryBinaryStream() = default;
        MemoryBinaryStream( const MemoryBinaryStream & ) = delete;
        MemoryBinaryStream( MemoryBinaryStream && ) = delete;

        MemoryBinaryStream &operator=( const MemoryBinaryStream & ) = delete;
        MemoryBinaryStream &operator=( MemoryBinaryStream && ) = delete;

        const primitive::Vector< Byte > &GetByteArray() const
        {
            SM_ASSERT_MSG( !IsOpen(), "Binary stream is open" );
            return m_ByteArray;
        }

        primitive::Vector< Byte > &GetByteArray()
        {
            SM_ASSERT_MSG( !IsOpen(), "Binary stream is open" );
            return m_ByteArray;
        }

        bool SetIndex( const Index index ) override;
        bool OpenInput() override;
        bool OpenOutput( OpeningModeFlags openingModeFlags ) override;
        bool OpenInputOutput( OpeningModeFlags openingModeFlags ) override;

        Count ReadByteArray( void *pByteArray, Count size ) override;
        Count WriteByteArray( const void *pByteArray, const Count size ) override;

        bool Close() override;

      private:
        primitive::Vector< Byte > m_ByteArray;
    };
}