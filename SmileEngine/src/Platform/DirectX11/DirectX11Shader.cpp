#include "smpch.h"
#include "DirectX11Shader.h"

#include "DirectX11Context.h"
#include "SmileEngine/Logger.h"
#include "DirectX11DataStructs.h"

namespace Smile
{
	DirectX11Shader::DirectX11Shader(DirectX11Context* pDirectX11Context, const std::string& assetFile, const std::string& techniqueName)
		: m_pDirectX11Context{ pDirectX11Context }
	{
		if (!LoadEffect(m_pDirectX11Context->GetDevice(), assetFile))
		{
			SAFE_RELEASE(m_pEffect);
			SM_ASSERT(false, "DirectX11Shader > Failed to load effect");
		}

		if (!techniqueName.empty())
			m_pTechnique = m_pEffect->GetTechniqueByName(techniqueName.c_str());
		else
			m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

		if (!m_pTechnique->IsValid())
			SM_LOG_WARNING("DirectX11Shader > Invalid technique");

		BuildInputLayout(ShaderLayouts::m_PosColLayout, 2);

		auto pEffectVariable = m_pEffect->GetVariableBySemantic("World");
		m_pWorldMatrixVariable = (pEffectVariable->IsValid()) ? pEffectVariable->AsMatrix() : nullptr;
		pEffectVariable = m_pEffect->GetVariableBySemantic("View");
		m_pViewMatrixVariable = (pEffectVariable->IsValid()) ? pEffectVariable->AsMatrix() : nullptr;
		pEffectVariable = m_pEffect->GetVariableBySemantic("ViewInverse");
		m_pViewInverseMatrixVariable = (pEffectVariable->IsValid()) ? pEffectVariable->AsMatrix() : nullptr;
		pEffectVariable = m_pEffect->GetVariableBySemantic("WorldViewProjection");
		m_pWorldViewProjMatrixVariable = (pEffectVariable->IsValid()) ? pEffectVariable->AsMatrix() : nullptr;

		LoadEffectVariables();
	}

	DirectX11Shader::~DirectX11Shader()
	{
		SAFE_RELEASE(m_pWorldMatrixVariable);
		SAFE_RELEASE(m_pViewMatrixVariable);
		SAFE_RELEASE(m_pViewInverseMatrixVariable);
		SAFE_RELEASE(m_pWorldViewProjMatrixVariable);
		SAFE_RELEASE(m_pInputLayout);
		SAFE_RELEASE(m_pTechnique);
		SAFE_RELEASE(m_pEffect);
	}

	void DirectX11Shader::Bind() const
	{
		m_pDirectX11Context->GetDeviceContext()->IASetInputLayout(m_pInputLayout);
	}

	void DirectX11Shader::Unbind() const
	{
		m_pDirectX11Context->GetDeviceContext()->IASetInputLayout(nullptr);
	}

	bool DirectX11Shader::LoadEffect(ID3D11Device* pDevice, const std::string& assetFile)
	{
		HRESULT result{ S_OK };
		ID3D10Blob* pErrorBlob{ nullptr };

		DWORD shaderFlags{ 0 };
#if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		std::wstring assetFileWStr{ assetFile.begin(), assetFile.end() };
		result = D3DX11CompileEffectFromFile(assetFileWStr.c_str(),
			nullptr,
			nullptr,
			shaderFlags,
			0,
			pDevice,
			&m_pEffect,
			&pErrorBlob);

		if (FAILED(result))
		{
			if (pErrorBlob)
			{
				char* pErrors{ (char*)pErrorBlob->GetBufferPointer() };

				std::wstringstream ss;
				for (unsigned int i{}; i < pErrorBlob->GetBufferSize(); ++i)
				{
					ss << pErrors[i];
				}

				OutputDebugStringW(ss.str().c_str());
				pErrorBlob->Release();
				pErrorBlob = nullptr;

				SM_LOG_ERROR("%s", ss.str());
			}
			else
			{
				SM_LOG_ERROR("DirectX11Shader::LoadEffect > Failed to CreateEffectFromFile: %s", assetFile);
				return false;
			}
		}

		return true;
	}

	void DirectX11Shader::BuildInputLayout(D3D11_INPUT_ELEMENT_DESC* pLayoutDesc, uint32_t count)
	{
		D3DX11_PASS_DESC passDesc{};
		m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
		HRESULT result = m_pDirectX11Context->GetDevice()->CreateInputLayout(pLayoutDesc, count, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout);

		if (FAILED(result))
			SM_LOG_ERROR("DirectX11Shader::BuildInputLayout > Failed to create input layout");
	}
}