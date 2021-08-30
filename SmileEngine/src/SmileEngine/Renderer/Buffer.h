#pragma once
#include "SmileEngine/Logger.h"

namespace Smile
{
	class RenderingContext;

	enum class ShaderDataType : uint8_t
	{
		eNone = 0,
		eFloat,
		eFloat2,
		eFloat3,
		eFloat4,
		eMat3,
		eMat4,
		eInt,
		eInt2,
		eInt3,
		eInt4,
		eBool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::eFloat:	return 4;
			case ShaderDataType::eFloat2:	return 8;
			case ShaderDataType::eFloat3:	return 12;
			case ShaderDataType::eFloat4:	return 16;
			case ShaderDataType::eMat3:		return 4 * 3 * 3;
			case ShaderDataType::eMat4:		return 4 * 4 * 4;
			case ShaderDataType::eInt:		return 4;
			case ShaderDataType::eInt2:		return 8;
			case ShaderDataType::eInt3:		return 12;
			case ShaderDataType::eInt4:		return 16;
			case ShaderDataType::eBool:		return 1;
			default:
				SM_ASSERT(false, "Buffer.h > ShaderDataTypeSize > Unknown ShaderDataType");
				return 0;
		}
	}

	struct BufferElement final
	{
		BufferElement() {}
		BufferElement(ShaderDataType type, const std::string& name, bool bNorm = false)
			: Name{ name }
			, Type{ type }
			, Size{ ShaderDataTypeSize(type) }
			, Offset{ 0 }
			, bNormalized{ bNorm }
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::eFloat:	return 1;
			case ShaderDataType::eFloat2:	return 2;
			case ShaderDataType::eFloat3:	return 3;
			case ShaderDataType::eFloat4:	return 4;
			case ShaderDataType::eMat3:		return 9;
			case ShaderDataType::eMat4:		return 16;
			case ShaderDataType::eInt:		return 1;
			case ShaderDataType::eInt2:		return 2;
			case ShaderDataType::eInt3:		return 3;
			case ShaderDataType::eInt4:		return 4;
			case ShaderDataType::eBool:		return 1;
			default:
				SM_ASSERT(false, "BufferElement::GetElementCount > Unknown ShaderDataType");
				return 0;
			}
		}

		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool bNormalized;
	};

	class SMILE_API BufferLayout final
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements{ elements } 
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.cbegin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.cend(); }

	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset{ 0 };
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class SMILE_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static VertexBuffer* Create(RenderingContext* pRenderingContext, void* pVertices, uint32_t count, const BufferLayout& layout);
	};

	class SMILE_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(RenderingContext* pRenderingContext, uint32_t* pIndices, uint32_t count);
	};
}