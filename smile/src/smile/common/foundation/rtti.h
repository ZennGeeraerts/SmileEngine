/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::foundation
{
    struct RTTI_IDBase
    {
    };

    using RTTI_ID = RTTI_IDBase;

    namespace detail
    {
        template < typename Type >
        struct RTTIUniqueValue final
        {
            static RTTI_ID Get()
            {
                static RTTI_IDBase value;
                return &value;
            }
        };
    }

#define RTTI_DEFINE_BASE( klass )                                                                                      \
    virtual smile::foundation::RTTI_ID GetRTTI_ID() const                                                              \
    {                                                                                                                  \
        return GetClassRTTI_ID();                                                                                      \
    }                                                                                                                  \
    virtual bool HasRTTI_ID( smile::foundation::RTTI_ID id ) const                                                     \
    {                                                                                                                  \
        return id == GetClassRTTI_ID();                                                                                \
    }                                                                                                                  \
    static smile::foundation::RTTI_ID GetClassRTTI_ID()                                                                \
    {                                                                                                                  \
        return smile::foundation::detail::RTTIUniqueValue< klass >::Get();                                             \
    }

#define RTTI_DEFINE( klass, base )                                                                                     \
    virtual smile::foundation::RTTI_ID GetRTTI_ID() const override                                                     \
    {                                                                                                                  \
        return GetClassRTTI_ID();                                                                                      \
    }                                                                                                                  \
    virtual bool HasRTTI_ID( smile::foundation::RTTI_ID id ) const override                                            \
    {                                                                                                                  \
        return id == GetClassRTTI_ID() || base::HasRTTI_ID( id );                                                      \
    }                                                                                                                  \
    static smile::foundation::RTTI_ID GetClassRTTI_ID()                                                                \
    {                                                                                                                  \
        return smile::foundation::detail::RTTIUniqueValue< klass >::Get();                                             \
    }

    template < typename Type, typename DerivedType >
    inline bool IsKindOf( const DerivedType &derived )
    {
        return derived.HasRTTI_ID( Type::GetClassRTTI_ID() );
    }

    template < typename Type, typename OtherType >
    inline bool IsA( const OtherType &other )
    {
        return other.GetRTTI_ID() == Type::GetClassRTTI_ID();
    }
}