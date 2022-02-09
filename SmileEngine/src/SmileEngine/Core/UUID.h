#pragma once

#include <xhash>

namespace Smile
{
	class UUID final
	{
	public:
		UUID();
		UUID(uint64_t id);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Smile::UUID>
	{
		size_t operator()(const Smile::UUID& uuid)
		{
			return hash<uint64_t>()(uuid);
		}
	};
}