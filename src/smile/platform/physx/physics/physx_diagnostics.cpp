/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "physx_diagnostics.h"

#include "logger/logging.h"

namespace smile::physics
{
    void
    PhysXErrorCallback::reportError( physx::PxErrorCode::Enum code, const char *message, const char *file, int line )
    {
        std::stringstream stringStream{};
        stringStream << "PHYSX CALLBACK REPORT:" << std::endl;
        stringStream << "Error Code: " << code << std::endl;
        stringStream << "Message: " << message << std::endl;
        stringStream << "File: " << file << " (line: " << line << ")";

        switch ( code )
        {
            case physx::PxErrorCode::eNO_ERROR:
            case physx::PxErrorCode::eDEBUG_INFO:
                SM_LOG_INFO( stringStream.str() );
                break;
            case physx::PxErrorCode::ePERF_WARNING:
            case physx::PxErrorCode::eDEBUG_WARNING:
                SM_LOG_WARNING( stringStream.str() );
                break;
            case physx::PxErrorCode::eINVALID_PARAMETER:
            case physx::PxErrorCode::eINVALID_OPERATION:
            case physx::PxErrorCode::eOUT_OF_MEMORY:
            case physx::PxErrorCode::eINTERNAL_ERROR:
                SM_LOG_ERROR( stringStream.str() );
                break;
            case physx::PxErrorCode::eABORT:
            case physx::PxErrorCode::eMASK_ALL:
                SM_LOG_CRITICALERROR( stringStream.str() );
                break;
            default:
                SM_LOG_ERROR( stringStream.str() );
                break;
        }
    }

    void PhysXAssertHandler::operator()( const char *exp, const char *file, int line, bool &ignore )
    {
#ifdef SM_DEBUG
        SM_LOG_CRITICALERROR( "[PhysX Error]: {0}:{1} - {2}", file, line, exp );
#endif
    }
}