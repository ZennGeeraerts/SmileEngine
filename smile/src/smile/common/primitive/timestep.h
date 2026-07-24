/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::primitive
{
    class Timestep final
    {
      public:
        constexpr Timestep( Real time = 0.0f ) : m_Time{ time }
        {
        }

        constexpr Real GetSeconds() const noexcept
        {
            return m_Time;
        }

        constexpr Real GetMilliseconds() const noexcept
        {
            return m_Time * 1000.f;
        }

        constexpr operator Real() const noexcept
        {
            return m_Time;
        }

        constexpr Timestep operator+( const Timestep &rhs ) const noexcept
        {
            return Timestep{ m_Time + rhs.m_Time };
        }

        constexpr Timestep operator-( const Timestep &rhs ) const noexcept
        {
            return Timestep{ m_Time - rhs.m_Time };
        }

        constexpr Timestep &operator+=( const Timestep &rhs ) noexcept
        {
            m_Time += rhs.m_Time;
            return *this;
        }

        constexpr Timestep &operator-=( const Timestep &rhs ) noexcept
        {
            m_Time -= rhs.m_Time;
            return *this;
        }

      private:
        Real m_Time;
    };
}

constexpr smile::primitive::Timestep operator"" _us( const unsigned long long microseconds ) noexcept
{
    return { static_cast< smile::Real >( microseconds ) / 1'000'000.0f };
}

constexpr smile::primitive::Timestep operator"" _us( const long double microseconds ) noexcept
{
    return { static_cast< smile::Real >( microseconds ) / 1'000'000.0f };
}
constexpr smile::primitive::Timestep operator"" _ms( const unsigned long long milliseconds ) noexcept
{
    return { static_cast< smile::Real >( milliseconds ) / 1'000.0f };
}

constexpr smile::primitive::Timestep operator"" _ms( const long double milliseconds ) noexcept
{
    return { static_cast< smile::Real >( milliseconds ) / 1'000.0f };
}

constexpr smile::primitive::Timestep operator"" _s( const unsigned long long seconds ) noexcept
{
    return { static_cast< smile::Real >( seconds ) };
}

constexpr smile::primitive::Timestep operator"" _s( const long double seconds ) noexcept
{
    return { static_cast< smile::Real >( seconds ) };
}
