/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::foundation
{
    template < typename ObjectType >
    class Singleton
    {
      public:
        static ObjectType &GetInstance()
        {
            static ObjectType instance{};
            return instance;
        }

      protected:
        Singleton() = default;
        Singleton( const Singleton & ) = delete;
        ~Singleton() = default;
        Singleton &operator=( const Singleton & ) = delete;
        Singleton &operator=( Singleton && ) = delete;
    };
}