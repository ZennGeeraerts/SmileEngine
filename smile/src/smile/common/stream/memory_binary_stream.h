/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "binary_stream.h"
#include "smile/common/memory/ref.h"

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

        const std::vector< Byte > &GetByteArray() const
        {
            SM_ASSERT_MSG( !IsOpen(), "Binary stream is open" );
            return m_ByteArray;
        }

        std::vector< Byte > &GetByteArray()
        {
            SM_ASSERT_MSG( !IsOpen(), "Binary stream is open" );
            return m_ByteArray;
        }

        bool SetIndex( const Uint32 index ) override;
        bool OpenInput() override;
        bool OpenOutput( OpeningModeFlags openingModeFlags ) override;
        bool OpenInputOutput( OpeningModeFlags openingModeFlags ) override;

        Uint32 ReadByteArray( void *pByteArray, Uint32 size ) override;
        Uint32 WriteByteArray( const void *pByteArray, const Uint32 size ) override;

        bool Close() override;

      private:
        std::vector< Byte > m_ByteArray;
    };
}