/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "modifier.h"

namespace smile::commandline
{
    void Description::Apply( OptionBase &option )
    {
        option.Description = Desc;
    }

    void Short::Apply( OptionBase &option )
    {
        option.ShortName = OneLetterOption;
    }
}