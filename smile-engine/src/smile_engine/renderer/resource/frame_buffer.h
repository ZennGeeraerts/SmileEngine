#pragma once

namespace smile
{
    enum class FramebufferTextureFormat
    {
        None = 0,
        RGBA8,
        DEPTH24STENCIL8,

        Depth = DEPTH24STENCIL8
    };

    struct FramebufferTextureData final
    {
        FramebufferTextureData() = default;
        FramebufferTextureData( FramebufferTextureFormat format, bool bUseShader = false )
            : m_TextureFormat{ format }, m_bUseInShader{ bUseShader }
        {
        }

        FramebufferTextureFormat m_TextureFormat = FramebufferTextureFormat::None;
        bool m_bUseInShader;
        // TODO: filtering/wrap
    };

    struct FramebufferAttachmentData final
    {
        FramebufferAttachmentData() = default;
        FramebufferAttachmentData( const std::initializer_list< FramebufferTextureData > &attachments )
            : m_Attachments{ attachments }
        {
        }

        std::vector< FramebufferTextureData > m_Attachments;
    };

    struct FramebufferData final
    {
        Uint32 m_Width = 0;
        Uint32 m_Height = 0;
        FramebufferAttachmentData m_Attachments;
        Uint16 m_Samples = 1;

        // if true -> Render to the swapchain
        bool m_bSwapChainTarget = false;
    };

    class Framebuffer
    {
      public:
        virtual ~Framebuffer() = default;

        virtual void Invalidate() = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void SetClearColor( const DirectX::XMFLOAT4 &color ) = 0;
        virtual void Clear() = 0;
        virtual void Resize( uint32_t width, uint32_t height ) = 0;

        virtual const FramebufferData &GetData() const = 0;
        virtual void *GetColor( uint32_t index ) const = 0;

        static Ref< Framebuffer > Create( const FramebufferData &framebufferData );
    };
}
