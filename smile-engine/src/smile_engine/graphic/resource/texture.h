#pragma once

#include "smile_engine/core/core.h"
#include <string>

namespace smile::graphic
{
    class Texture
    {
      public:
        virtual ~Texture() = default;

        virtual const std::string &getFilePath() const = 0;
        virtual Uint32 getWidth() const = 0;
        virtual Uint32 getHeight() const = 0;
        virtual void *getData() const = 0;
    };

    class Texture2D : public Texture
    {
      public:
        virtual ~Texture2D() = default;

        static Ref< Texture2D > create( const std::string &file_path );
    };
}
