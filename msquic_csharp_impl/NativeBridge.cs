using System.Runtime.InteropServices;

namespace msquic_csharp_impl
{
    internal static class NativeBridge
    {
        [DllImport("Native_Lib", EntryPoint = "run_c_server", CallingConvention = CallingConvention.Cdecl)]
        private static extern void RunCServerNative();

        internal static void RunCServer()
        {
            RunCServerNative();
        }
    }
}