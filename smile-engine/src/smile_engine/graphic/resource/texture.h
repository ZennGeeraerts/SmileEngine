#pragma once

#include "smile_engine/core/core.h"
#include <string>

namespace smile::graphic
{
    class Texture
    {
      public:
        virtual ~Texture() = default;

        virtual const std::string &GetFilePath() const = 0;
        virtual Uint32 GetWidth() const = 0;
        virtual Uint32 GetHeight() const = 0;
        virtual void *GetData() const = 0;
    };

    class Texture2D : public Texture
    {
      public:
        virtual ~Texture2D() = default;

        static Ref< Texture2D > Create( const std::string &filePath );
    };
}
