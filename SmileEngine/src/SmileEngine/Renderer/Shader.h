#pragma once

#include "SmileEngine/Core.h"
#include "Buffer.h"

namespace Smile
{
	class RenderingContext;

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadMat4(const std::string& sementicName, const DirectX::XMFLOAT4X4& matrix) = 0;
		virtual void UploadFloat3(const std::string& sementicName, const DirectX::XMFLOAT3& value) = 0;

		static Shader* Create(RenderingContext* pRenderingContext, const std::string& filePath, const BufferLayout& bufferLayout);
	};
}

