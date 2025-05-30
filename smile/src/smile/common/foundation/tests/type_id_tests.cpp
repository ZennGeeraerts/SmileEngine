/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/foundation/type_id.h"

struct MyStruct;
class MyClass;

namespace typeidtest
{
    using namespace smile;

    struct MyStruct;
    class MyClass;

    static_assert( foundation::TypeNameOf< int >() == "int" );
    static_assert( foundation::TypeNameOf< bool >() == "bool" );
    static_assert( foundation::TypeNameOf< float >() == "float" );

    static_assert( foundation::TypeNameOf< ::MyStruct >() == "MyStruct" );
    static_assert( foundation::TypeNameOf< ::MyClass >() == "MyClass" );

    static_assert( foundation::TypeNameOf< MyStruct >() == "typeidtest::MyStruct" );
    static_assert( foundation::TypeNameOf< MyClass >() == "typeidtest::MyClass" );

    static_assert( foundation::TypeIDOf< int >() == foundation::TypeIDOf( "int" ) );
    static_assert( foundation::TypeIDOf< bool >() == foundation::TypeIDOf< bool >() );
    static_assert( foundation::TypeIDOf< int >() != foundation::TypeIDOf< bool >() );
    static_assert( foundation::TypeIDOf< int >() != foundation::TypeIDOf< float >() );
}