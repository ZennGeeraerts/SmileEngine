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
		m_pShader->UploadFloat3("Albedo", albedo);
		m_Albedo = albedo;
	}

	void Material::SetAlbedo(const Ref<Texture2D>& pAlbedoMap)
	{
		m_pShader->UploadTexture2D("AlbedoMap", pAlbedoMap);
		m_pAlbedoMap = pAlbedoMap;
	}

	void Material::SetMetalness(float metalness)
	{
		m_pShader->UploadFloat("Metalness", metalness);
		m_Metalness = metalness;
	}

	void Material::SetMetalness(const Ref<Texture2D>& pMetalnessMap)
	{
		m_pShader->UploadTexture2D("MetalnessMap", pMetalnessMap);
		m_pMetalnessMap = pMetalnessMap;
	}

	void Material::SetRoughness(float roughness)
	{
		m_pShader->UploadFloat("Roughness", roughness);
		m_Roughness = roughness;
	}

	void Material::SetRoughness(const Ref<Texture2D>& pRoughnessMap)
	{
		m_pShader->UploadTexture2D("RoughnessMap", pRoughnessMap);
		m_pRoughnessMap = pRoughnessMap;
	}

	void Material::SetNormalMap(const Ref<Texture2D>& pNormalMap)
	{
		m_pShader->UploadTexture2D("NormalMap", pNormalMap);
		m_pNormalMap = pNormalMap;
	}

	void Material::SetAOMap(const Ref<Texture2D>& pAOMap)
	{
		m_pShader->UploadTexture2D("AOMap", pAOMap);
		m_pAOMap = pAOMap;
	}

	void Material::SetUseAlbedoMap(bool bUse)
	{
		if (bUse != m_bUseAlbedoMap)
		{
			m_pShader->UploadBool("UseAlbedoMap", bUse);
			m_bUseAlbedoMap = bUse;
		}
	}

	void Material::SetUseMetalnessMap(bool bUse)
	{
		if (bUse != m_bUseMetalnessMap)
		{
			m_pShader->UploadBool("UseMetalnessMap", bUse);
			m_bUseMetalnessMap = bUse;
		}
	}

	void Material::SetUseRoughnessMap(bool bUse)
	{
		if (bUse != m_bUseRoughnessMap)
		{
			m_pShader->UploadBool("UseRoughnessMap", bUse);
			m_bUseRoughnessMap = bUse;
		}
	}

	void Material::SetUseNormalMap(bool bUse)
	{
		if (bUse != m_bUseNormalMap)
		{
			m_pShader->UploadBool("UseNormalMap", bUse);
			m_bUseNormalMap = bUse;
		}
	}

	void Material::SetUseAOMap(bool bUse)
	{
		if (bUse != m_bUseAOMap)
		{
			m_pShader->UploadBool("UseAOMap", bUse);
			m_bUseAOMap = bUse;
		}
	}
}