#pragma once
#include "SmileEngine/Renderer/Shader.h"

namespace Smile
{
	class SmileRasterContext;

	class SmileRasterShader final : public Shader
	{
	public:
		SmileRasterShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return "SmileRasterShader"; }

		virtual void UploadMat4(const std::string& sementicName, const DirectX::XMFLOAT4X4& matrix) override;
		virtual void UploadMat4Array(const std::string& sementicName, const std::vector<DirectX::XMFLOAT4X4>& matArray) override;
		virtual void UploadFloat3(const std::string& sementicName, const DirectX::XMFLOAT3& value) override;
		virtual void UploadInt(const std::string& sementicName, int value) override;
		virtual void UploadTexture2D(const std::string& sementicName, const Ref<Texture2D>& pTexture2D) override;
		virtual void UploadBool(const std::string& sementicName, bool value) override;
		virtual void UploadFloat(const std::string& sementicName, float value) override;

	private:
		SmileRasterContext* m_pSmileRasterContext = nullptr;

		mutable std::unordered_map<std::string, DirectX::XMFLOAT4X4> m_Matrices{};
		DirectX::XMFLOAT4X4 m_ViewProjectionMatrix{};
		DirectX::XMFLOAT4X4 m_WorldMatrix{};
		DirectX::XMFLOAT4X4 m_ViewInverseMatrix{};
	};
}

