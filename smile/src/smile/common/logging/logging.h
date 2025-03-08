/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "logger_registry.h"

// Macro's are used for the log functions, so the functionality can easily be removed if we want a distribution build
#define SM_LOG_TRACE( ... ) ::smile::logging::LoggerRegistry::GetInstance().GetDefaultLogger()->Trace( __VA_ARGS__ )
#define SM_LOG_DEBUG( ... ) ::smile::logging::LoggerRegistry::GetInstance().GetDefaultLogger()->Debug( __VA_ARGS__ )
#define SM_LOG_INFO( ... ) ::smile::logging::LoggerRegistry::GetInstance().GetDefaultLogger()->Info( __VA_ARGS__ )
#define SM_LOG_WARNING( ... ) ::smile::logging::LoggerRegistry::GetInstance().GetDefaultLogger()->Warning( __VA_ARGS__ )
#define SM_LOG_ERROR( ... ) ::smile::logging::LoggerRegistry::GetInstance().GetDefaultLogger()->Error( __VA_ARGS__ )
#define SM_LOG_CRITICALERROR( ... )                                                                                    \
    ::smile::logging::LoggerRegistry::GetInstance().GetDefaultLogger()->CriticalError( __VA_ARGS__ )