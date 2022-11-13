#include "smile_engine/stl/type_id.h"

struct MyStruct;
class MyClass;

namespace typeidtest
{
    using namespace smile;

    struct MyStruct;
    class MyClass;

    static_assert( stl::TypeNameOf< int >() == "int" );
    static_assert( stl::TypeNameOf< bool >() == "bool" );
    static_assert( stl::TypeNameOf< float >() == "float" );

    static_assert( stl::TypeNameOf< ::MyStruct >() == "MyStruct" );
    static_assert( stl::TypeNameOf< ::MyClass >() == "MyClass" );

    static_assert( stl::TypeNameOf< MyStruct >() == "typeidtest::MyStruct" );
    static_assert( stl::TypeNameOf< MyClass >() == "typeidtest::MyClass" );

    static_assert( stl::TypeIDOf< int >() == stl::TypeIDOf( "int" ) );
    static_assert( stl::TypeIDOf< bool >() == stl::TypeIDOf< bool >() );
    static_assert( stl::TypeIDOf< int >() != stl::TypeIDOf< bool >() );
    static_assert( stl::TypeIDOf< int >() != stl::TypeIDOf< float >() );
}