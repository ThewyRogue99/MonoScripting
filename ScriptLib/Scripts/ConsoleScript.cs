using System.Threading;

namespace ScriptLib
{
    public class ConsoleScript : IScript
    {
        public int Count = 0;
        public string Name = "Yarrak";

        public void OnBegin()
        {
            GameConsole.WriteLine("Begin Play");
        }

        public void OnUpdate()
        {
            GameConsole.WriteLine("Update");

            GameConsole.WriteLine("Name: " + Name);

            Thread.Sleep(1000);
        }
    }
}
