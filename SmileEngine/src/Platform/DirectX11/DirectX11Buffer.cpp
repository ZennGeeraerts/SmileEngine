#include "smpch.h"
#include "DirectX11Buffer.h"

#include "SmileEngine/Logger.h"
#include "Platform/DirectX11/DirectX11Context.h"

namespace Smile
{
	//*------------------------------------------------------------------------------------------------*//
	//*----------------------------------------- Vertex buffer -----------------------------------------*//
	//*------------------------------------------------------------------------------------------------*//

	DirectX11VertexBuffer::DirectX11VertexBuffer(DirectX11Context* pDirectX11Context, void* pVertices, uint32_t count, const BufferLayout& layout)
		: m_pDirectX11Context{ pDirectX11Context }
		, m_Layout{ layout }
	{
		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = layout.GetStride() * count;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = { 0 };
		initData.pSysMem = pVertices;

		HRESULT result = pDirectX11Context->GetDevice()->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
		if (FAILED(result))
		{
			SM_LOG_ERROR("DirectX11VertexBuffer > Failed to create vertex buffer");
			return;
		}
	}

	DirectX11VertexBuffer::~DirectX11VertexBuffer()
	{
		SAFE_RELEASE(m_pVertexBuffer);
	}

	void DirectX11VertexBuffer::Bind() const
	{
		uint32_t offset{ 0 };
		uint32_t stride{ m_Layout.GetStride() };
		m_pDirectX11Context->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	}

	void DirectX11VertexBuffer::Unbind() const
	{
		m_pDirectX11Context->GetDeviceContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	}

	//*------------------------------------------------------------------------------------------------*//
	//*----------------------------------------- Index buffer -----------------------------------------*//
	//*------------------------------------------------------------------------------------------------*//

	DirectX11IndexBuffer::DirectX11IndexBuffer(DirectX11Context* pDirectX11Context, uint32_t* pIndices, uint32_t count)
		: m_pDirectX11Context{ pDirectX11Context }
	{
		m_AmountIndices = count;

		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(uint32_t) * count;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = { 0 };
		initData.pSysMem = pIndices;

		HRESULT result = pDirectX11Context->GetDevice()->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
		if (FAILED(result))
		{
			SM_LOG_ERROR("DirectX11IndexBuffer > Failed to create index buffer");
			return;
		}
	}

	DirectX11IndexBuffer::~DirectX11IndexBuffer()
	{
		SAFE_RELEASE(m_pIndexBuffer);
	}

	void DirectX11IndexBuffer::Bind() const
	{
		m_pDirectX11Context->GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void DirectX11IndexBuffer::Unbind() const
	{
		m_pDirectX11Context->GetDeviceContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	}
}