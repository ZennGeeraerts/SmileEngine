#pragma once

namespace smile
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
        CullMode m_CullMode = CullMode::None;
        FillMode m_FillMode = FillMode::Solid;
        bool m_bDepthClipEnable = true;
    };

    class RasterizerState
    {
      public:
        virtual ~RasterizerState() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Ref< RasterizerState > Create( const RasterizerStateData &rasterizerStateData );
    };
}