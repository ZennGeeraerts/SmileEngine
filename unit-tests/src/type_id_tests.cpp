#include "smile_engine/common/compiled/type_id.h"

struct MyStruct;
class MyClass;

namespace typeidtest
{
    using namespace smile;

    struct MyStruct;
    class MyClass;

    static_assert( compiled::TypeNameOf< int >() == "int" );
    static_assert( compiled::TypeNameOf< bool >() == "bool" );
    static_assert( compiled::TypeNameOf< float >() == "float" );

    static_assert( compiled::TypeNameOf< ::MyStruct >() == "MyStruct" );
    static_assert( compiled::TypeNameOf< ::MyClass >() == "MyClass" );

    static_assert( compiled::TypeNameOf< MyStruct >() == "typeidtest::MyStruct" );
    static_assert( compiled::TypeNameOf< MyClass >() == "typeidtest::MyClass" );

    static_assert( compiled::TypeIDOf< int >() == compiled::TypeIDOf( "int" ) );
    static_assert( compiled::TypeIDOf< bool >() == compiled::TypeIDOf< bool >() );
    static_assert( compiled::TypeIDOf< int >() != compiled::TypeIDOf< bool >() );
    static_assert( compiled::TypeIDOf< int >() != compiled::TypeIDOf< float >() );
}