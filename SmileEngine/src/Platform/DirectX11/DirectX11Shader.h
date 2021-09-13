#pragma once
#include "SmileEngine/Renderer/Shader.h"
#include "SmileEngine/Renderer/Buffer.h"
#include "DirectX11Texture.h"

namespace Smile
{
	class DirectX11Context;

	class DirectX11Shader final : public Shader
	{
	public:
		DirectX11Shader(const std::string& assetFile, const BufferLayout& layout, const std::string& techniqueName = "");
		virtual ~DirectX11Shader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadMat4(const std::string& sementicName, const DirectX::XMFLOAT4X4& matrix) override;
		virtual void UploadFloat3(const std::string& sementicName, const DirectX::XMFLOAT3& value) override;
		virtual void UploadInt(const std::string& sementicName, int value) override;
		virtual void UploadTexture2D(const std::string& sementicName, const Ref<Texture2D>& pTexture2D) override;

		inline ID3DX11Effect* GetEffect() const { return m_pEffect; }
		inline ID3DX11EffectTechnique* GetTechnique() const { return m_pTechnique; }

	private:
		bool LoadEffect(ID3D11Device* pDevice, const std::string& assetFile);
		void BuildInputLayout(const BufferLayout& layout);
		DXGI_FORMAT ShaderDataTypeToDirectXBaseType(ShaderDataType type);
		ID3DX11EffectVariable* GetEffectVariable(const std::string& sementicName);

	private:
		std::unordered_map<std::string, ID3DX11EffectVariable*> m_EffectVariableMap;
		DirectX11Context* m_pDirectX11Context;

		ID3DX11Effect* m_pEffect;
		ID3DX11EffectTechnique* m_pTechnique;
		ID3D11InputLayout* m_pInputLayout;
	};
}

