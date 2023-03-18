using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Smile
{
    public struct Vector3
    {
        public float X, Y, Z;
        public static Vector3 Zero = new Vector3(0);

        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static Vector3 operator+(Vector3 lhs, Vector3 rhs)
        {
            return new Vector3(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
        }
        public static Vector3 operator*(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }
    }

    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string text, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogVector(ref Vector3 parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GetEntityTranslation(ulong entityID, out Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetEntityTranslation(ulong entityID, ref Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyPressed(KeyCode keyCode);
    }
    public class Entity
    {
        protected Entity()
        {
            ID = 0;
        }

        internal Entity(ulong id)
        {
            ID = id;
        }

        public readonly ulong ID;

        public Vector3 Translation
        {
            get 
            {
                InternalCalls.GetEntityTranslation(ID, out Vector3 translation);
                return translation;
            }
            set 
            { 
                InternalCalls.SetEntityTranslation(ID, ref value); 
            }
        }
    }
}
