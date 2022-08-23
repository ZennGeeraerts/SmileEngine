#include "smile_engine/core/compiled/type_id.h"

struct MyStruct;
class MyClass;

namespace typeidtest
{
    using namespace smile;

    struct MyStruct;
    class MyClass;

    static_assert( typeNameOf< int >() == "int" );
    static_assert( typeNameOf< bool >() == "bool" );
    static_assert( typeNameOf< float >() == "float" );

    static_assert( typeNameOf< ::MyStruct >() == "MyStruct" );
    static_assert( typeNameOf< ::MyClass >() == "MyClass" );

    static_assert( typeNameOf< MyStruct >() == "typeidtest::MyStruct" );
    static_assert( typeNameOf< MyClass >() == "typeidtest::MyClass" );

    static_assert( typeIDOf< int >() == typeIDOf( "int" ) );
    static_assert( typeIDOf< bool >() == typeIDOf< bool >() );
    static_assert( typeIDOf< int >() != typeIDOf< bool >() );
    static_assert( typeIDOf< int >() != typeIDOf< float >() );
}