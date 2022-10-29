#include "smile_engine/core/compiled/type_id.h"

struct MyStruct;
class MyClass;

namespace typeidtest
{
    using namespace Smile;

    struct MyStruct;
    class MyClass;

    static_assert( TypeNameOf< int >() == "int" );
    static_assert( TypeNameOf< bool >() == "bool" );
    static_assert( TypeNameOf< float >() == "float" );

    static_assert( TypeNameOf< ::MyStruct >() == "MyStruct" );
    static_assert( TypeNameOf< ::MyClass >() == "MyClass" );

    static_assert( TypeNameOf< MyStruct >() == "typeidtest::MyStruct" );
    static_assert( TypeNameOf< MyClass >() == "typeidtest::MyClass" );

    static_assert( TypeIDOf< int >() == TypeIDOf( "int" ) );
    static_assert( TypeIDOf< bool >() == TypeIDOf< bool >() );
    static_assert( TypeIDOf< int >() != TypeIDOf< bool >() );
    static_assert( TypeIDOf< int >() != TypeIDOf< float >() );
}