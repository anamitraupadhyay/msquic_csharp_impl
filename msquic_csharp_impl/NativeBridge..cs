using System.Runtime.InteropServices;

namespace msquic_csharp_impl
{
    internal static partial class NativeBridge
    {
        // VS 2026 / .NET 10 uses LibraryImport for better performance over DllImport
        [LibraryImport("native_lib", EntryPoint = "run_c_server")]
        internal static partial void RunCServer();
    }
}
