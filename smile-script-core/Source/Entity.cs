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

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    }

    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string text, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogVector(ref Vector3 parameter);
    }
    class Entity
    {
        public float FloatVar { get; set; }
        public Entity()
        {
            Console.WriteLine("Main constructor!");
            InternalCalls.NativeLog("Zenn", 8085);

            Vector3 pos = new Vector3(5, 2.5f, 3);
            InternalCalls.NativeLogVector(ref pos);
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello world from C#");
        }

        public void PrintInt(int value)
        {
            Console.WriteLine($"Integer value: {value}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# custom message: {message}");
        }
    }
}
