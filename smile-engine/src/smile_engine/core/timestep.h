#pragma once

namespace smile
{
    class Timestep final
    {
      public:
        Timestep( float time = 0.0f ) : time{ time }
        {
        }

        float getSeconds() const
        {
            return time;
        }
        float getMilliseconds() const
        {
            return time * 1000.f;
        }

      public:
        operator float() const
        {
            return time;
        }
        Timestep operator+( const Timestep &rhs )
        {
            return Timestep{ time += rhs.time };
        }
        Timestep operator-( const Timestep &rhs )
        {
            return Timestep{ time -= rhs.time };
        }
        Timestep operator+=( const Timestep &rhs )
        {
            time += rhs.time;
            return *this;
        }
        Timestep operator-=( const Timestep &rhs )
        {
            time -= rhs.time;
            return *this;
        }

      private:
        float time;
    };
}
