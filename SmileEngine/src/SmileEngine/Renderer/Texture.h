#pragma once

#include "SmileEngine/Core/Core.h"
#include <string>

namespace Smile
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual const std::string& GetFilePath() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void* GetData() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Ref<Texture2D> Create(const std::string& filePath);
	};
}

