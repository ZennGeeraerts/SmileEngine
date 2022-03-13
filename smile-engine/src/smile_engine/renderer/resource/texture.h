#pragma once

#include "smile_engine/core/core.h"
#include <string>

namespace smile
{
    class Texture
    {
      public:
        virtual ~Texture() = default;

        virtual const std::string &GetFilePath() const = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual void *GetData() const = 0;
    };

    class Texture2D : public Texture
    {
      public:
        virtual ~Texture2D() = default;

        static Ref< Texture2D > Create( const std::string &filePath );
    };
}
