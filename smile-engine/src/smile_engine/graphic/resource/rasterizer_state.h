#pragma once

namespace smile::graphic
{
    enum class CullMode
    {
        None,
        Front,
        Back
    };

    enum class FillMode
    {
        WireFrame,
        Solid
    };

    struct RasterizerStateDescriptor final
    {
        CullMode CullMode = CullMode::None;
        FillMode FillMode = FillMode::Solid;
        bool EnableDepthClip = true;
    };

    class RasterizerState
    {
      public:
        virtual ~RasterizerState() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Ref< RasterizerState > Create( const RasterizerStateDescriptor &rasterizerStateDesc );
    };
}