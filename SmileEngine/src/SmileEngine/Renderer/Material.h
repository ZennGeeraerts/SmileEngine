#pragma once
#include "Shader.h"

#include "Texture.h"

namespace Smile
{
	class Material final
	{
	public:
		enum class RenderingMode
		{
			eSpecular,
			eMetallic
		};

		virtual ~Material() = default;

	private:
		Ref<Texture2D> m_pAlbedoMap = nullptr;
		Ref<Texture2D> m_pNormalMap = nullptr;
		Ref<Texture2D> m_pAOMap = nullptr;

		Ref<Texture2D> m_pSpecularMap = nullptr;
		Ref<Texture2D> m_pMetalnessMap = nullptr;
	};
}

