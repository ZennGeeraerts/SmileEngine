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
        FramebufferTextureData( FramebufferTextureFormat format, bool use_in_shader = false )
            : textureFormat{ format }, useInShader{ use_in_shader }
        {
        }

        FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
        bool useInShader;
        // TODO: filtering/wrap
    };

    struct FramebufferAttachmentData final
    {
        FramebufferAttachmentData() = default;
        FramebufferAttachmentData( const std::initializer_list< FramebufferTextureData > &attachments )
            : attachments{ attachments }
        {
        }

        std::vector< FramebufferTextureData > attachments;
    };

    struct FramebufferData final
    {
        Uint32 width = 0;
        Uint32 height = 0;
        FramebufferAttachmentData attachments;
        Uint16 samples = 1;

        // if true -> Render to the swapchain
        bool swapChainTarget = false;
    };

    class Framebuffer
    {
      public:
        virtual ~Framebuffer() = default;

        virtual void invalidate() = 0;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void setClearColor( const DirectX::XMFLOAT4 &color ) = 0;
        virtual void clear() = 0;
        virtual void resize( Uint32 width, Uint32 height ) = 0;

        virtual const FramebufferData &getData() const = 0;
        virtual void *getColor( Uint32 index ) const = 0;

        static Ref< Framebuffer > create( const FramebufferData &frame_buffer_data );
    };
}
