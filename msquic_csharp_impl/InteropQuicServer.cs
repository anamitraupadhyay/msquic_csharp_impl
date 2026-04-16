using Microsoft.Quic;

namespace msquic_csharp_impl
{
    public class InteropQuicServer
    {
        private unsafe QUIC_API_TABLE* _apiTable;

        public unsafe void Start()
        {
            try
            {
                // Open MSQUIC API
                _apiTable = MsQuic.Open();
                Console.WriteLine("MSQUIC initialized successfully");
            }
            catch (MsQuicException ex)
            {
                Console.WriteLine($"Failed to initialize MSQUIC: {ex.Message}");
                throw;
            }
        }

        public unsafe void Stop()
        {
            if (_apiTable != null)
            {
                MsQuic.Close(_apiTable);
                _apiTable = null;
            }
        }
    }
}