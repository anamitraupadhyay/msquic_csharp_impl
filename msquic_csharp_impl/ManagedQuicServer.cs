//using Microsoft.Quic; thought of the namespace in the code
using System.Net.Quic;

namespace msquic_csharp_impl
{
    public class ManagedQuicServer
    {
        // The return type of an async method must be void, Task, Task<T>,
        // a task-like type, IAsyncEnumerable<T>, or IAsyncEnumerator<T>
        internal async /*Task<object>*/ void StartAsync()
        {
            QuicConnection connection = null;
            QuicListener listener = null;
            QuicStream streamer = null;
            // important if some goes unnoticed and should trigger fordev.log, todo for later
            throw new NotImplementedException();
        }
    }
}
