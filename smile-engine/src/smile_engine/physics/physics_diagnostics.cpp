#include "smpch.h"
#include "physics_diagnostics.h"

#include "smile_engine/core/logger.h"

namespace smile
{
    void PhysicsErrorCallback::reportError( physx::PxErrorCode::Enum code,
        const char *pMessage,
        const char *pFile,
        int line )
    {
        std::stringstream stringStream{};
        stringStream << "PHYSX CALLBACK REPORT:" << std::endl;
        stringStream << "Error Code: " << code << std::endl;
        stringStream << "Message: " << pMessage << std::endl;
        stringStream << "File: " << pFile << " (line: " << line << ")";

        switch ( code )
        {
            case physx::PxErrorCode::eNO_ERROR:
            case physx::PxErrorCode::eDEBUG_INFO:
                SM_LOG_INFO( stringStream.str().c_str() );
                break;
            case physx::PxErrorCode::ePERF_WARNING:
            case physx::PxErrorCode::eDEBUG_WARNING:
                SM_LOG_WARNING( stringStream.str().c_str() );
                break;
            case physx::PxErrorCode::eINVALID_PARAMETER:
            case physx::PxErrorCode::eINVALID_OPERATION:
            case physx::PxErrorCode::eOUT_OF_MEMORY:
            case physx::PxErrorCode::eINTERNAL_ERROR:
                SM_LOG_ERROR( stringStream.str().c_str() );
            case physx::PxErrorCode::eABORT:
            case physx::PxErrorCode::eMASK_ALL:
                SM_LOG_CRITICALERROR( stringStream.str().c_str() );
            default:
                SM_LOG_ERROR( stringStream.str().c_str() );
                break;
        }
    }

    void PhysicsAssertHandler::operator()( const char *pExp, const char *pFile, int line, bool &bIgnore )
    {
#ifdef SM_DEBUG
        SM_LOG_CRITICALERROR( "[PhysX Error]: %s:%d - %s", pFile, line, pExp );
#endif
    }
}