#pragma once

namespace Smile
{
    class Timestep final
    {
      public:
        Timestep( float time = 0.0f ) : m_Time{ time }
        {
        }

        float GetSeconds() const
        {
            return m_Time;
        }
        float GetMilliseconds() const
        {
            return m_Time * 1000.f;
        }

      public:
        operator float() const
        {
            return m_Time;
        }
        Timestep operator+( const Timestep &rhs )
        {
            return Timestep{ m_Time += rhs.m_Time };
        }
        Timestep operator-( const Timestep &rhs )
        {
            return Timestep{ m_Time -= rhs.m_Time };
        }
        Timestep operator+=( const Timestep &rhs )
        {
            m_Time += rhs.m_Time;
            return *this;
        }
        Timestep operator-=( const Timestep &rhs )
        {
            m_Time -= rhs.m_Time;
            return *this;
        }

      private:
        float m_Time;
    };
}
