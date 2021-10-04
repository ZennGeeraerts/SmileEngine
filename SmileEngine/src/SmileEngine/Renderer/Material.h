#pragma once
#include "Shader.h"

#include "Texture.h"

namespace Smile
{
	class Material final
	{
	public:
		Material();
		virtual ~Material() = default;

		void SetAlbedo(const DirectX::XMFLOAT3& albedo);
		void SetAlbedo(const Ref<Texture2D>& pAlbedoMap);

		void SetMetalness(float metalness);
		void SetMetalness(const Ref<Texture2D>& pMetalnessMap);

		void SetRoughness(float roughness);
		void SetRoughness(const Ref<Texture2D>& pRoughnessMap);

		void SetNormalMap(const Ref<Texture2D>& pNormalMap);
		void SetAOMap(const Ref<Texture2D>& pAOMap);

		const BufferLayout& GetBufferLayout() const { return m_BufferLayout; }
		const Ref<Shader>& GetShader() const { return m_pShader; }

	private:
		DirectX::XMFLOAT3 m_Albedo = { 0.f, 0.f, 0.f };
		float m_Metalness = 0.0f;
		float m_Roughness = 0.5f;

		Ref<Texture2D> m_pAlbedoMap = nullptr;
		Ref<Texture2D> m_pMetalnessMap = nullptr;
		Ref<Texture2D> m_pRoughnessMap = nullptr;
		Ref<Texture2D> m_pNormalMap = nullptr;
		Ref<Texture2D> m_pAOMap = nullptr;

		bool m_bUseAlbedoMap = false;
		bool m_bUseMetalnessMap = false;
		bool m_bUseRoughnessMap = false;
		bool m_bUseNormalMap = false;
		bool m_bUseAOMap = false;

		BufferLayout m_BufferLayout;
		Ref<Shader> m_pShader = nullptr;
	};
}

