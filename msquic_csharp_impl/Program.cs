using msquic_csharp_impl;
using System.Reflection.PortableExecutable;
public class printfuncCollections
{
    public void PrintfuncChoices()
    {
        Console.WriteLine("quic server instance type choices:");
        Console.WriteLine("1. managed ie the library astracted c#");
        Console.WriteLine("2. interop ie the low level interoped c# with c");
        Console.WriteLine("3. pure c msquic imlplementation");
    }
    public void PrintInitofManagedServer()
    {
        Console.WriteLine("c# managed server inited configs are loading");
    }
    public void PrintInitofInteropServer() {
        Console.WriteLine("c# interop server inited with configs");
    }
    public void PrintInitofPureServer()
    {
        Console.WriteLine("c based server inited with configs");
    }
}

namespace msquic_csharp_impl
{
    class Program
    {
        static async Task Main(string[] args)
        {
            printfuncCollections print = new printfuncCollections();
            print.PrintfuncChoices();// the print instance to remain new'd as it will be handler in main level
            ConsoleKeyInfo choice = Console.ReadKey(true);
            char serverChoice = choice.KeyChar;

            Console.WriteLine($"\nStarting Mode: {serverChoice}\n");
            switch (serverChoice) {
                case '1':
                    // c# but managed one
                    print.PrintInitofManagedServer();
                    ManagedQuicServer managedQuicServer = new ManagedQuicServer();
                    /*await*/ managedQuicServer.StartAsync();// this commented version means it will get out after exec
                    // that is launched and got out of main loopthat doesnt make an any sense so the func call shouldnt
                    // be async that is the startasync is that so also since its start generally its kept void
                    // and its outputting void cant be awaitted so...
                    break;

                case '2':
                    // Interop Way 
                    print.PrintInitofInteropServer();
                    unsafe
                    {
                        InteropQuicServer.Start();
                    }
                    break;

                case '3':
                    // Pure C Bridge
                    print.PrintInitofPureServer();
                    NativeBridge.RunCServer();
                    break;

                default:
                    Console.WriteLine("Invalid selection. Terminating.");
                    break;
            }
        }
    }
}
