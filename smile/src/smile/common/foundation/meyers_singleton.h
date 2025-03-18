/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::foundation
{
    template < typename ObjectType >
    class MeyersSingleton
    {
      public:
        static ObjectType &GetInstance()
        {
            static ObjectType instance{};
            return instance;
        }

      protected:
        MeyersSingleton() = default;
        MeyersSingleton( const MeyersSingleton & ) = delete;
        ~MeyersSingleton() = default;
        MeyersSingleton &operator=( const MeyersSingleton & ) = delete;
        MeyersSingleton &operator=( MeyersSingleton && ) = delete;
    };
}