#include "smpch.h"
#include "SmileRasterShader.h"

#include "SmileRasterContext.h"
#include "SmileEngine/Core/SmileGame.h"

namespace Smile
{
	SmileRasterShader::SmileRasterShader(const std::string& assetFile, const BufferLayout& layout)
	{
		m_pSmileRasterContext = static_cast<SmileRasterContext*>(SmileGame::GetInstance().GetWindow().GetRenderingContext());
		SM_ASSERT(m_pSmileRasterContext, "SmileRasterShader > Rendering context is not a SmileRaster Rendering Context");

		// Find name from asset path
		auto lastSlash = assetFile.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = assetFile.rfind('.');
		auto count = lastDot == std::string::npos ? assetFile.size() - lastSlash : lastDot - lastSlash;
		m_Name = assetFile.substr(lastSlash, count);
	}

	void SmileRasterShader::Bind() const
	{

	}

	void SmileRasterShader::Unbind() const
	{

	}

	void SmileRasterShader::UploadMat4(const std::string& sementicName, const DirectX::XMFLOAT4X4& matrix)
	{
		m_pSmileRasterContext->GetDeviceContext()->UploadMat4(sementicName, matrix);
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