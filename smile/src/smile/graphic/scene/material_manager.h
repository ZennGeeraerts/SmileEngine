#pragma once

#include "smile/common/foundation/singleton.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "material_asset.h"

namespace smile::graphic
{
    class MaterialManager final : public memory::Counted, public foundation::Singleton< MaterialManager >
    {
      public:
        MaterialManager() = default;
        ~MaterialManager() = default;

      private:
        primitive::HashMap< Material::Ref, MaterialAsset::Ref > m_Materials;
        // TextureLoader m_TextureLoader;
        // memory::Ref< TextureAsset > m_pFallBackTexture;
    };
}