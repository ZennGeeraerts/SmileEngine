using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Smile
{
    public class Input
    {
        public static bool IsKeyPressed(KeyCode keyCode)
        {
            return InternalCalls.Input_IsKeyPressed(keyCode);
        }
    }
}
