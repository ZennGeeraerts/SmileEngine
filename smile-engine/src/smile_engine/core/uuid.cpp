#include "smpch.h"
#include "uuid.h"

#include <random>

namespace smile
{
    static std::random_device randomDevice{};
    static std::mt19937_64 engine{ randomDevice() };
    static std::uniform_int_distribution< uint64_t > uniformDistribution{};

    UUID::UUID() : uuid{ uniformDistribution( engine ) }
    {
    }

    UUID::UUID( Uint64 id ) : uuid{ id }
    {
    }
}