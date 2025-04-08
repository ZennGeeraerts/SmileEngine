/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/memory/ref.h"

#include <catch/catch.hpp>

namespace smile
{
    class AllocatedStackFixture
    {
      public:
        AllocatedStackFixture()
        {
            m_pAllocatedObject = new memory::Counted;
            m_pNonConstAllocatedObject = new memory::Counted;
        }

        AllocatedStackFixture( const AllocatedStackFixture & ) = delete;
        AllocatedStackFixture &operator=( const AllocatedStackFixture & ) = delete;

        ~AllocatedStackFixture() = default;

        memory::Ref< memory::Counted > GetNonConstRValue()
        {
            return m_pNonConstAllocatedObject;
        }

        memory::Ref< const memory::Counted > m_pAllocatedObject, m_pEmptyRef;
        memory::Ref< memory::Counted > m_pNonConstAllocatedObject;
    };

    TEST_CASE_METHOD( AllocatedStackFixture, "memory::Ref copy constructor behavior", "[memory]" )
    {
        memory::Ref< const memory::Counted > pEmptyCopy{ m_pEmptyRef }, pAllocatedObject{ m_pAllocatedObject },
            pNonConstStackObjectRValue{ GetNonConstRValue() }, pNonConstAllocatedObject{ m_pNonConstAllocatedObject };

        REQUIRE( 2 == pAllocatedObject->GetRefCount() );
        REQUIRE( 3 == pNonConstAllocatedObject->GetRefCount() );
        REQUIRE( nullptr == m_pEmptyRef.GetPointer() );
    }

    TEST_CASE_METHOD( AllocatedStackFixture, "memory::Ref copy operator", "[memory]" )
    {
        memory::Ref< const memory::Counted > pEmptyCopy, pAllocatedObject;

        pEmptyCopy = m_pEmptyRef;
        pAllocatedObject = m_pAllocatedObject;

        REQUIRE( 2 == pAllocatedObject->GetRefCount() );
        REQUIRE( nullptr == m_pEmptyRef.GetPointer() );

        pAllocatedObject = m_pNonConstAllocatedObject;

        REQUIRE( 2 == pAllocatedObject->GetRefCount() );

        pAllocatedObject = GetNonConstRValue();
        REQUIRE( 2 == pAllocatedObject->GetRefCount() );
    }

    TEST_CASE_METHOD( AllocatedStackFixture, "memory::Ref equal operator", "[memory]" )
    {
        memory::Ref< const memory::Counted > pEmptyCopy{ m_pEmptyRef }, pAllocatedObject{ m_pAllocatedObject };

        REQUIRE( pAllocatedObject == m_pAllocatedObject );
        REQUIRE( m_pEmptyRef == pEmptyCopy );
        REQUIRE( !( m_pAllocatedObject == pEmptyCopy ) );
    }

    TEST_CASE_METHOD( AllocatedStackFixture, "memory::Ref not equal operator", "[memory]" )
    {
        memory::Ref< const memory::Counted > pEmptyCopy{ m_pEmptyRef }, pAllocatedObject{ m_pAllocatedObject };

        REQUIRE( !( pAllocatedObject != m_pAllocatedObject ) );
        REQUIRE( !( pEmptyCopy != m_pEmptyRef ) );
        REQUIRE( m_pAllocatedObject != pEmptyCopy );
    }

    struct LocalConstnessTest : public memory::Counted
    {
        bool TestConstness() const
        {
        }
        int TestConstness()
        {
        }
    };

    static_assert( sizeof( memory::Ref< const LocalConstnessTest >()->TestConstness() ) == 1 );
    static_assert(
        sizeof( memory::Ref< const LocalConstnessTest >().GetObject< LocalConstnessTest >().TestConstness() ) == 1 );

    struct Dummy final : memory::Counted
    {
        explicit Dummy( bool &temp ) : Temp{ temp }
        {
        }

        ~Dummy() override
        {
            Temp = false;
        }

        bool &Temp;
    };

    TEST_CASE( "SelfAssign", "[memory]" )
    {
        bool temp = false;
        memory::Ref< Dummy > pDummy;

        temp = true;

        pDummy = new Dummy{ temp };

        pDummy.Reset( pDummy.GetPointer() );

        REQUIRE( temp );
    }

    TEST_CASE( "SetToNull", "[memory]" )
    {
        bool temp = false;
        memory::Ref< Dummy > pDummy;

        temp = true;

        pDummy = new Dummy{ temp };

        pDummy.Reset( nullptr );

        REQUIRE_FALSE( temp );
    }
}    