#pragma once
#include "Shader.h"

#include "Texture.h"

namespace Smile
{
	class Material
	{
	public:
		virtual ~Material() = default;

		virtual void Bind() const = 0;
		virtual void SetTexture2D(const std::string sementicName, const Ref<Texture2D>& pTexture) = 0;

		static Ref<Material> Create(const Ref<Shader>& pShader);
	};
}

