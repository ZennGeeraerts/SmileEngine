#include "smpch.h"
#include "Material.h"

namespace Smile
{
	Material::Material()
	{
		m_BufferLayout =
		{
			{ ShaderDataType::eFloat3, "Position" },
			{ ShaderDataType::eFloat3, "Normal" },
			{ ShaderDataType::eFloat2, "TexCoord" },
			{ ShaderDataType::eFloat3, "Tangent" }
		};

		m_pShader = Shader::Create("Resources/Shaders/PBR.fx", m_BufferLayout);

		auto pEnvironmentMap = Texture2D::Create("Resources/Textures/Sunol_Cubemap.dds");
		m_pShader->UploadTexture2D("EnvironmentMap", pEnvironmentMap);
	}

	void Material::SetAlbedo(const DirectX::XMFLOAT3& albedo)
	{
		if (m_bUseAlbedoMap)
		{
			m_pShader->UploadBool("UseAlbedoMap", false);
			m_bUseAlbedoMap = false;
		}

		m_pShader->UploadFloat3("Albedo", albedo);
		m_Albedo = albedo;
	}

	void Material::SetAlbedo(const Ref<Texture2D>& pAlbedoMap)
	{
		if (!m_bUseAlbedoMap)
		{
			m_pShader->UploadBool("UseAlbedoMap", true);
			m_bUseAlbedoMap = true;
		}

		m_pShader->UploadTexture2D("AlbedoMap", pAlbedoMap);
		m_pAlbedoMap = pAlbedoMap;
	}

	void Material::SetMetalness(float metalness)
	{
		if (m_bUseMetalnessMap)
		{
			m_pShader->UploadBool("UseMetalnessMap", false);
			m_bUseMetalnessMap = false;
		}

		m_pShader->UploadFloat("Metalness", metalness);
		m_Metalness = metalness;
	}

	void Material::SetMetalness(const Ref<Texture2D>& pMetalnessMap)
	{
		if (!m_bUseMetalnessMap)
		{
			m_pShader->UploadBool("UseMetalnessMap", true);
			m_bUseMetalnessMap = true;
		}

		m_pShader->UploadTexture2D("MetalnessMap", pMetalnessMap);
		m_pMetalnessMap = pMetalnessMap;
	}

	void Material::SetRoughness(float roughness)
	{
		if (m_bUseRoughnessMap)
		{
			m_pShader->UploadBool("UseRoughnessMap", false);
			m_bUseRoughnessMap = false;
		}

		m_pShader->UploadFloat("Roughness", roughness);
		m_Roughness = roughness;
	}

	void Material::SetRoughness(const Ref<Texture2D>& pRoughnessMap)
	{
		if (!m_bUseRoughnessMap)
		{
			m_pShader->UploadBool("UseRoughnessMap", true);
			m_bUseRoughnessMap = true;
		}

		m_pShader->UploadTexture2D("RoughnessMap", pRoughnessMap);
		m_pRoughnessMap = pRoughnessMap;
	}

	void Material::SetNormalMap(const Ref<Texture2D>& pNormalMap)
	{
		if (!m_bUseNormalMap)
		{
			m_pShader->UploadBool("UseNormalMap", true);
			m_bUseNormalMap = true;
		}

		m_pShader->UploadTexture2D("NormalMap", pNormalMap);
		m_pNormalMap = pNormalMap;
	}

	void Material::SetAOMap(const Ref<Texture2D>& pAOMap)
	{
		if (!m_bUseAOMap)
		{
			m_pShader->UploadBool("UseAOMap", true);
			m_bUseAOMap = true;
		}

		m_pShader->UploadTexture2D("AOMap", pAOMap);
		m_pAOMap = pAOMap;
	}
}