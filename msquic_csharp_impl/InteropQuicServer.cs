using Microsoft.Quic;// the vanilla declaration

namespace msquic_csharp_impl
{
    public class InteropQuicServer
    {
        private unsafe QUIC_API_TABLE* _apiTable;//holder for the api table pointer

        public unsafe void Start()
        {
            try
            {
                // Open MSQUIC API
                _apiTable = MsQuic.Open();// signature: public static unsafe QUIC_API_TABLE* Open();
                // the init sequence is: look at last end of this func
                Console.WriteLine("MSQUIC initialized successfully");
            }
            catch (MsQuicException ex)
            {
                Console.WriteLine($"Failed to initialize MSQUIC: {ex.Message}");
                throw;
            }
        }

        public unsafe void Stop()// is it auto called by the program exit or should be called explicitly?
        { // the answer is it should be called explicitly
            if (_apiTable != null)
            {
                MsQuic.Close(_apiTable);
                _apiTable = null;
            }
        }
    }
}

/*
the actual vars and func exposed to us library programmers is QUIC_API_TABLE* the raw declaration.
and in the func is .open() and .close() iykyk. the open func does slews of operation which cant be classified as only arena allocator.
we need to track down the absolute path of .open() to get a better understanding of msquic_api_table
here it goes .open() by us and then open() by msquic is calling msquicversionopen which passes our 
QUIC_API_TABLE* and version 2 if otherwise it will fail by logging. and internally msquicopenversion
calls msquiclibraryload in alongside msquicopenversion library.c ok this is a bit different as it inits global vars but contains so many compiler macrosto direct them to platform specific code it becomes a rabbit hole so avoid those library loading that is the extern QUIC_LIBRARY MsQuicLib;
in library.h almost all are quic library specific and some safeties too in that struct except
CXPLAT_DATAPATH* Datapath; CXPLAT_WORKER_POOL* WorkerPool; CXPLAT_LIST_ENTRY Registrations;
CXPLAT_LIST_ENTRY Bindings; these 4 are the most crucial below is more general theory

Datapath receives a raw UDP packet from the OS. as misleading it is its socket damn microslop
It looks at the Bindings list to find the QUIC_BINDING matching the local IP/Port.
The Binding looks at the QUIC Connection ID (CID) in the packet header to find the specific QUIC_CONNECTION.
The packet is queued to a Worker inside the WorkerPool (chosen based on the PartitionID inside the CID).
The Worker thread wakes up, decrypts the packet, and updates the state.
The Registration linked to that connection is used to find the "App Callback" to tell my code that data recieved just like aftergettingframe in live555 rtspclient
*/
