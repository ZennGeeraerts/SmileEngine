#include "smpch.h"
#include "file_descriptor_list.h"

#include "smile/common/primitive/text/utils.h"
#include "smile/common/primitive/collection/array_utils.h"

namespace smile::fs
{
    namespace detail
    {
        bool CompareFileDescriptorLower( const FileDescriptor &first, const primitive::StringView second )
        {
            return first.LogicalName < second;
        }

        bool CompareFileDescriptorUpper( const primitive::StringView first, const FileDescriptor &second )
        {
            return first < second.LogicalName;
        }
    }

    std::optional< Path > FileDescriptorList::FindPhysicalFilePath( const Path &filePath ) const
    {
        auto entry = FindDescriptor( filePath );

        if ( !entry.has_value() )
        {
            return std::nullopt;
        }

        SM_ASSERT( !entry->IsVirtual );

        return entry->GetPhysicalFilePath();
    }

    std::optional< FileDescriptor > FileDescriptorList::FindDescriptor( const Path &filePath ) const
    {
        primitive::String fileName = filePath.GetFileName();
        primitive::String directory = filePath.GetDirectory();

        if ( !m_IsCaseSensitive )
        {
            primitive::ToLowerCase( fileName );
            primitive::ToLowerCase( directory );
        }

        const auto lowerBound =
            primitive::array::GetLowerBoundIndex( m_Descriptors, fileName, detail::CompareFileDescriptorLower );

        const auto upperBound =
            primitive::array::GetUpperBoundIndex( m_Descriptors, fileName, detail::CompareFileDescriptorUpper );

        if ( lowerBound < upperBound )
        {
            for ( Index index = lowerBound; index < upperBound; ++index )
            {
                const FileDescriptor &descriptor = m_Descriptors[index];

                if ( descriptor.LogicalDirectoryPath.GetText() == directory )
                {
                    return descriptor;
                }
            }
        }

        return std::nullopt;
    }
}