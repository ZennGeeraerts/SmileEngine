#pragma once

namespace smile::ecs
{
    using ConstructorType = void ( * )( void * );
    using DestructorType = void ( * )( void * );

    template < typename Type, typename... Args >
    inline void rawConstructObject( void *object, Args &&...params )
    {
        new ( object ) Type( std::forward< Args >( params )... );
    }

    template < typename Type >
    inline void rawDestructObject( void *object )
    {
        static_cast< Type * >( object )->Type::~Type();
    }

    class ComponentStorage
    {
     public:
        int getSize() const
        {
            return size;
        }

     protected:
        template < typename ComponentType >
        void initialize()
        {
            data = nullptr;
            size = 0;
            allocated = 0;
            constructor = rawConstructObject< ComponentType >();
            destructor = rawDestructObject< ComponentType >();
            cSize = sizeof( ComponentType );
        }

        int cSize = 0;
        int allocated = 0;
        int size = 0;
        char *data = nullptr;
        int *indices = nullptr;

        ConstructorType constructor = nullptr;
        DestructorType destructor = nullptr;
    };
}