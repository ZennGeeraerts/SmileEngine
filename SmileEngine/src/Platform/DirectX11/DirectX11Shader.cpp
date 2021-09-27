#include "smpch.h"
#include "DirectX11Shader.h"

#include "DirectX11Context.h"
#include "SmileEngine/Core/Logger.h"
#include "SmileEngine/Core/SmileGame.h"

namespace Smile
{
	DirectX11Shader::DirectX11Shader(const std::string& assetFile, const BufferLayout& layout, const std::string& techniqueName)
	{
		m_pDirectX11Context = static_cast<DirectX11Context*>(SmileGame::GetInstance().GetWindow().GetRenderingContext());
		SM_ASSERT(m_pDirectX11Context, "DirectX11Shader > Rendering context is not a DirectX 11 Rendering Context");

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

		BuildInputLayout(layout);

		// Find name from asset path
		auto lastSlash = assetFile.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = assetFile.rfind('.');
		auto count = lastDot == std::string::npos ? assetFile.size() - lastSlash : lastDot - lastSlash;
		m_Name = assetFile.substr(lastSlash, count);
	}

	DirectX11Shader::~DirectX11Shader()
	{
		for (auto& effectVar : m_EffectVariableMap)
		{
			SAFE_RELEASE(effectVar.second);
		}
		m_EffectVariableMap.clear();

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

	void DirectX11Shader::UploadMat4(const std::string& sementicName, const DirectX::XMFLOAT4X4& matrix)
	{
		auto pMatrixVariable = GetEffectVariable(sementicName)->AsMatrix();
		if (pMatrixVariable->IsValid())
		{
			pMatrixVariable->SetMatrix(&matrix._11);
		}
	}

	void DirectX11Shader::UploadFloat3(const std::string& sementicName, const DirectX::XMFLOAT3& value)
	{
		auto pVectorVariable = GetEffectVariable(sementicName)->AsVector();
		if (pVectorVariable->IsValid())
		{
			pVectorVariable->SetFloatVector(&value.x);
		}
	}

	void DirectX11Shader::UploadInt(const std::string& sementicName, int value)
	{
		auto pIntVariable = GetEffectVariable(sementicName)->AsScalar();
		if (pIntVariable->IsValid())
		{
			pIntVariable->SetInt(value);
		}
	}

	void DirectX11Shader::UploadTexture2D(const std::string& sementicName, const Ref<Texture2D>& pTexture2D)
	{
		auto pTextureVariable = GetEffectVariable(sementicName)->AsShaderResource();
		if (pTextureVariable->IsValid())
		{
			pTextureVariable->SetResource((ID3D11ShaderResourceView*)pTexture2D->GetData());
		}
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
			}

			return false;
		}

		return true;
	}

	void DirectX11Shader::BuildInputLayout(const BufferLayout& layout)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputDescs{};
		for (const auto& element : layout)
		{
			inputDescs.push_back(
				D3D11_INPUT_ELEMENT_DESC
				{ 
					element.Name.c_str(), 0, ShaderDataTypeToDirectXBaseType(element.Type), 0, element.Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 
				});
		}

		uint32_t count{ static_cast<uint32_t>(inputDescs.size()) };

		D3DX11_PASS_DESC passDesc{};
		m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
		HRESULT result = m_pDirectX11Context->GetDevice()->CreateInputLayout(inputDescs.data(), count, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout);

		if (FAILED(result))
			SM_LOG_ERROR("DirectX11Shader::BuildInputLayout > Failed to create input layout");
	}

	DXGI_FORMAT DirectX11Shader::ShaderDataTypeToDirectXBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::eFloat:	return DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::eFloat2:	return DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::eFloat3:	return DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::eFloat4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::eMat3:		return DXGI_FORMAT_UNKNOWN;
		case ShaderDataType::eMat4:		return DXGI_FORMAT_UNKNOWN;
		case ShaderDataType::eInt:		return DXGI_FORMAT_R32_SINT;
		case ShaderDataType::eInt2:		return DXGI_FORMAT_R32G32_SINT;
		case ShaderDataType::eInt3:		return DXGI_FORMAT_R32G32B32_SINT;
		case ShaderDataType::eInt4:		return DXGI_FORMAT_R32G32B32A32_SINT;
		case ShaderDataType::eBool:		return DXGI_FORMAT_UNKNOWN;
		default:
			SM_ASSERT(false, "ShaderDataTypeToDirectXBaseType > Unknown ShaderDataType");
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	ID3DX11EffectVariable* DirectX11Shader::GetEffectVariable(const std::string& sementicName)
	{
		if (m_EffectVariableMap.find(sementicName) != m_EffectVariableMap.end())
			return m_EffectVariableMap[sementicName];

		auto pEffectVariable = m_pEffect->GetVariableBySemantic(sementicName.c_str());
		if (!pEffectVariable->IsValid())
		{
			SM_LOG_WARNING("DirectX11Shader::GetEffectVariable > Invalid effect variable: %s", sementicName);
			return nullptr;
		}

		m_EffectVariableMap[sementicName] = pEffectVariable;
		return pEffectVariable;
	}
}