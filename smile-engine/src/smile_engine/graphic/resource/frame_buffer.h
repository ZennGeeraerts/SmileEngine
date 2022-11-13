#pragma once

namespace smile::graphic
{
    enum class FramebufferTextureFormat
    {
        None = 0,
        RGBA8,
        Depth24Stencil8,

        Depth = Depth24Stencil8
    };

    struct FramebufferTextureData final
    {
        FramebufferTextureData() = default;
        FramebufferTextureData( FramebufferTextureFormat format, bool useInShader = false )
            : TextureFormat{ format }, UseInShader{ useInShader }
        {
        }

        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
        bool UseInShader;
        // TODO: filtering/wrap
    };

    struct FramebufferAttachmentData final
    {
        FramebufferAttachmentData() = default;
        FramebufferAttachmentData( const std::initializer_list< FramebufferTextureData > &attachments )
            : Attachments{ attachments }
        {
        }

        std::vector< FramebufferTextureData > Attachments;
    };

    struct FramebufferDescriptor final
    {
        Uint32 Width = 0;
        Uint32 Height = 0;
        FramebufferAttachmentData Attachments;
        Uint16 Samples = 1;

        // if true -> Render to the swapchain
        bool IsSwapChainTarget = false;
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
        virtual void Resize( Uint32 width, Uint32 height ) = 0;

        virtual const FramebufferDescriptor &GetDescriptor() const = 0;
        virtual void *GetColor( Uint32 index ) const = 0;

        static Ref< Framebuffer > Create( const FramebufferDescriptor &framebufferDesc );
    };
}
