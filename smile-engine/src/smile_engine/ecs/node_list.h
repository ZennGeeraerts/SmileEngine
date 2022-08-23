#pragma once

#include "component_interface.h"

#include "smile_engine/core/compiled/type_id.h"

namespace smile::ecs
{
	template< typename... Components >
	class NodeList final
	{
      private:
        static constexpr compiled::TypeID typeIDs[] = { typeIDOf< Components >()... };

        std::vector< ComponentInterface * > components{};
	};
}