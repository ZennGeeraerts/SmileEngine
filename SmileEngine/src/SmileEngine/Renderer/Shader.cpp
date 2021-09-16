#include "smpch.h"
#include "Shader.h"

#include "Renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#include "Platform/DirectX11/DirectX11Context.h"
#include "Platform/DirectX11/DirectX11Shader.h"
#endif

namespace Smile
{
	Ref<Shader> Shader::Create(const std::string& filePath, const BufferLayout& bufferLayout)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::eNone:
			SM_ASSERT(false, "Shader::Create > return nullptr, no renderer api selected");
			return nullptr;

#ifdef SM_PLATFORM_WINDOWS
		case RendererAPI::API::eDirectX11:
			return std::make_shared<DirectX11Shader>(filePath, bufferLayout);
#endif
		}

		SM_ASSERT(false, "Shader::Create > return nullptr, unknown render api or render api is not supported on this platform");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& pShader)
	{
		SM_ASSERT(!Exists(name), "ShaderLibrary::Add > Shader: %s already exists!", name);
		m_Shaders[name] = pShader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& pShader)
	{
		auto& name = pShader->GetName();
		Add(name, pShader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath, const BufferLayout& bufferLayout)
	{
		auto pShader = Shader::Create(filePath, bufferLayout);
		Add(pShader);
		return pShader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath, const BufferLayout& bufferLayout)
	{
		auto pShader = Shader::Create(filePath, bufferLayout);
		Add(name, pShader);
		return pShader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		SM_ASSERT(Exists(name), "ShaderLibrary::Add > Shader: %s not found!", name);
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}