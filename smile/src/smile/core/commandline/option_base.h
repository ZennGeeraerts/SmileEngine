/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::commandline
{
    class Arg;
    struct OptionMap;

    class OptionBase
    {
      public:
        explicit OptionBase( const char *name );
        virtual ~OptionBase() = default;

        bool IsPositional() const;

        virtual bool Parse( const Arg &arg ) = 0;

        static OptionMap &GetOptionMap();

        void *operator new( std::size_t ) = delete;

      public:
        const char *Description{ nullptr };
        const char *Name{ nullptr };
        char ShortName{ 0 };

      protected:
        void AppendOption();

      protected:
        OptionBase *m_pNext{ nullptr };

        inline static OptionBase *s_pOptionList{ nullptr };
    };
}