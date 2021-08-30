#pragma once

#include "SmileEngine/Core.h"
#include "Buffer.h"

namespace Smile
{
	class RenderingContext;

	class SMILE_API Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void LoadEffectVariables() = 0;
		virtual void UploadMat4(const std::string& sementicName, const DirectX::XMFLOAT4X4& matrix) = 0;

		static Shader* Create(RenderingContext* pRenderingContext, const std::string& filePath, const BufferLayout& bufferLayout);
	};
}

