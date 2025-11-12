/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "option_base.h"

#include "smile/common/memory/scope.h"
#include "arguments.h"
#include "option_map.h"

namespace smile::commandline
{
    bool g_OptionListIsDirty = false;
    memory::Scope< OptionMap > g_pOptionMap{ nullptr };

    OptionBase::OptionBase( const char *name ) : Name{ name }
    {
    }

    bool OptionBase::IsPositional() const
    {
        return Name == nullptr;
    }

    void OptionBase::AppendOption()
    {
        m_pNext = s_pOptionList;
        s_pOptionList = this;
        g_OptionListIsDirty = true;
    }

    OptionMap &OptionBase::GetOptionMap()
    {
        if ( !g_pOptionMap )
        {
            g_pOptionMap = memory::CreateScope< OptionMap >();
        }

        if ( g_OptionListIsDirty )
        {
            g_pOptionMap->Options.Clear();
            g_pOptionMap->ShortOptions.Clear();
            g_pOptionMap->PositionalOption = nullptr;

            OptionBase *pCurrentOption = s_pOptionList;

            while ( pCurrentOption )
            {
                if ( pCurrentOption->Name )
                {
                    g_pOptionMap->Options.Insert( pCurrentOption->Name, pCurrentOption );
                }

                if ( pCurrentOption->ShortName )
                {
                    g_pOptionMap->ShortOptions.Insert( pCurrentOption->ShortName, pCurrentOption );
                }

                if ( !pCurrentOption->Name && !pCurrentOption->ShortName )
                {
                    SM_ASSERT( !g_pOptionMap->PositionalOption, "Only 1 positional option is allowed" );
                    g_pOptionMap->PositionalOption = pCurrentOption;
                }

                pCurrentOption = pCurrentOption->m_pNext;
            }

            g_OptionListIsDirty = false;
        }

        return *g_pOptionMap;
    }
}