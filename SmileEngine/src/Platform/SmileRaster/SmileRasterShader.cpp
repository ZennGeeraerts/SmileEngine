#include "smpch.h"
#include "SmileRasterShader.h"

#include "SmileRasterContext.h"
#include "SmileEngine/Core/SmileGame.h"

namespace Smile
{
	SmileRasterShader::SmileRasterShader()
	{
		m_pSmileRasterContext = static_cast<SmileRasterContext*>(SmileGame::GetInstance().GetWindow().GetRenderingContext());
		SM_ASSERT(m_pSmileRasterContext, "SmileRasterShader > Rendering context is not a SmileRaster Rendering Context");
	}

	void SmileRasterShader::Bind() const
	{
		DirectX::XMFLOAT4X4 viewProjectionMatrix{};
		DirectX::XMStoreFloat4x4(&viewProjectionMatrix, DirectX::XMMatrixIdentity());
		if (m_Matrices.find("ViewProjection") != m_Matrices.end())
			viewProjectionMatrix = m_Matrices["ViewProjection"];

		DirectX::XMFLOAT4X4 worldMatrix{};
		DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
		if (m_Matrices.find("World") != m_Matrices.end())
			worldMatrix = m_Matrices["World"];

		DirectX::XMFLOAT4X4 viewInverseMatrix{};
		DirectX::XMStoreFloat4x4(&viewInverseMatrix, DirectX::XMMatrixIdentity());
		if (m_Matrices.find("ViewInverse") != m_Matrices.end())
			viewInverseMatrix = m_Matrices["ViewInverse"];

		m_pSmileRasterContext->GetDeviceContext()->SetShaderData(viewProjectionMatrix, worldMatrix, viewInverseMatrix);
	}

	void SmileRasterShader::Unbind() const
	{

	}

	void SmileRasterShader::UploadMat4(const std::string& sementicName, const DirectX::XMFLOAT4X4& matrix)
	{
		m_Matrices.insert(std::make_pair(sementicName, matrix));
	}

	void SmileRasterShader::UploadMat4Array(const std::string& sementicName, const std::vector<DirectX::XMFLOAT4X4>& matArray)
	{

	}

	void SmileRasterShader::UploadFloat3(const std::string& sementicName, const DirectX::XMFLOAT3& value)
	{

	}

	void SmileRasterShader::UploadInt(const std::string& sementicName, int value)
	{

	}

	void SmileRasterShader::UploadTexture2D(const std::string& sementicName, const Ref<Texture2D>& pTexture2D)
	{

	}

	void SmileRasterShader::UploadBool(const std::string& sementicName, bool value)
	{

	}

	void SmileRasterShader::UploadFloat(const std::string& sementicName, float value)
	{

	}
}