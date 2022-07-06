using System.Runtime.CompilerServices;

namespace ScriptLib
{
    public class GameConsole
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Write(string str);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void WriteLine(string str);
    }
}
