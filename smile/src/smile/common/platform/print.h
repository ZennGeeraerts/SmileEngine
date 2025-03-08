/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::platform
{
    void Print( const char *text, Uint32 length );
    const char *EOL();
    void SetConsoleColor( Uint16 attributes );
}