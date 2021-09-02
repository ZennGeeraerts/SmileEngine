#pragma once
#include "SmileEngine/Renderer/Shader.h"
#include "SmileEngine/Renderer/Buffer.h"

namespace Smile
{
	class DirectX11Context;

	class DirectX11Shader : public Shader
	{
	public:
		DirectX11Shader(DirectX11Context* pDirectX11Context, const std::string& assetFile, const BufferLayout& layout, const std::string& techniqueName = "");
		virtual ~DirectX11Shader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadMat4(const std::string& sementicName, const DirectX::XMFLOAT4X4& matrix) override;
		virtual void UploadFloat3(const std::string& sementicName, const DirectX::XMFLOAT3& value) override;

		inline ID3DX11Effect* GetEffect() const { return m_pEffect; }
		inline ID3DX11EffectTechnique* GetTechnique() const { return m_pTechnique; }

	private:
		bool LoadEffect(ID3D11Device* pDevice, const std::string& assetFile);
		void BuildInputLayout(const BufferLayout& layout);
		DXGI_FORMAT ShaderDataTypeToDirectXBaseType(ShaderDataType type);

	private:
		DirectX11Context* m_pDirectX11Context;

		ID3DX11Effect* m_pEffect;
		ID3DX11EffectTechnique* m_pTechnique;

		/*ID3DX11EffectMatrixVariable* m_pWorldMatrixVariable;
		ID3DX11EffectMatrixVariable* m_pViewMatrixVariable;
		ID3DX11EffectMatrixVariable* m_pViewInverseMatrixVariable;
		ID3DX11EffectMatrixVariable* m_pWorldViewProjMatrixVariable;*/

		ID3D11InputLayout* m_pInputLayout;
	};
}

