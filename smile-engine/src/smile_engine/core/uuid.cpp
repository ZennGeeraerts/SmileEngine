#include "smpch.h"
#include "uuid.h"

#include <random>

namespace Smile
{
    static std::random_device s_RandomDevice{};
    static std::mt19937_64 s_Engine{ s_RandomDevice() };
    static std::uniform_int_distribution< Uint64 > s_UniformDistribution{};

    UUID::UUID() : m_UUID{ s_UniformDistribution( s_Engine ) }
    {
    }

    UUID::UUID( Uint64 id ) : m_UUID{ id }
    {
    }
}