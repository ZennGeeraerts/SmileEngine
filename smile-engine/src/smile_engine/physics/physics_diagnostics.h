#pragma once
#include <foundation/PxErrorCallback.h>
#include <foundation/PxAssert.h>

namespace smile
{
    class PhysicsErrorCallback final : public physx::PxErrorCallback
    {
      public:
        virtual void
        reportError( physx::PxErrorCode::Enum code, const char *pMessage, const char *pFile, int line ) override;
    };

    class PhysicsAssertHandler final : public physx::PxAssertHandler
    {
        virtual void operator()( const char *pExp, const char *pFile, int line, bool &bIgnore );
    };
}