#pragma once

#include "SmileEngine/Core.h"
#include "Buffer.h"
#include "Texture.h"

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
		virtual void UploadInt(const std::string& sementicName, int value) = 0;
		virtual void UploadTexture2D(const std::string& sementicName, const Ref<Texture2D>& pTexture2D) = 0;

		static Shader* Create(const std::string& filePath, const BufferLayout& bufferLayout);
	};
}

