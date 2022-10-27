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

    struct RasterizerStateData final
    {
        CullMode cullMode = CullMode::None;
        FillMode fillMode = FillMode::Solid;
        bool depthClipEnable = true;
    };

    class RasterizerState
    {
      public:
        virtual ~RasterizerState() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        static Ref< RasterizerState > create( const RasterizerStateData &rasterizer_state_data );
    };
}