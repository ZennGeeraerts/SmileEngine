/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "memory/ref.h"

#include <catch/catch.hpp>

namespace smile
{
    class AllocatedStackFixture
    {
      public:
        AllocatedStackFixture()
        {
            m_pAllocatedObject = new memory::Object;
            m_pNonConstAllocatedObject = new memory::Object;
        }

        AllocatedStackFixture( const AllocatedStackFixture & ) = delete;
        AllocatedStackFixture &operator=( const AllocatedStackFixture & ) = delete;

        ~AllocatedStackFixture() = default;

        memory::Ref< memory::Object > GetNonConstRValue()
        {
            return m_pNonConstAllocatedObject;
        }

        memory::Ref< const memory::Object > m_pAllocatedObject, m_pEmptyRef;
        memory::Ref< memory::Object > m_pNonConstAllocatedObject;
    };

    TEST_CASE_METHOD( AllocatedStackFixture, "memory::Ref copy constructor behavior", "[memory]" )
    {
        memory::Ref< const memory::Object > pEmptyCopy{ m_pEmptyRef }, pAllocatedObject{ m_pAllocatedObject },
            pNonConstStackObjectRValue{ GetNonConstRValue() }, pNonConstAllocatedObject{ m_pNonConstAllocatedObject };

        REQUIRE( 2 == pAllocatedObject->GetRefCount() );
        REQUIRE( 3 == pNonConstAllocatedObject->GetRefCount() );
        REQUIRE( nullptr == m_pEmptyRef.GetPointer() );
    }
}