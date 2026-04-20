#include <msquic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define NATIVE_API __declspec(dllexport)
#else
#define NATIVE_API
#endif

const QUIC_API_TABLE* MsQuic;
HQUIC GlobalConfiguration = NULL;

QUIC_STATUS QUIC_API ServerStreamCallback(HQUIC Stream, void* Context, QUIC_STREAM_EVENT* Event);
QUIC_STATUS QUIC_API ServerConnectionCallback(HQUIC Connection, void* Context, QUIC_CONNECTION_EVENT* Event);
QUIC_STATUS QUIC_API ServerListenerCallback(HQUIC Listener, void* Context, QUIC_LISTENER_EVENT* Event);

//The Stream Callback: This is "afterGettingFrame"
//Triggered for every chunk of data on a specific stream.
QUIC_STATUS QUIC_API ServerStreamCallback(HQUIC Stream, void* Context, QUIC_STREAM_EVENT* Event) {
    switch (Event->Type) {
    case QUIC_STREAM_EVENT_RECEIVE:
        //Zero-copy. 
        //process the buffers directly from MSQUIC's internal memory.
        for (uint32_t i = 0; i < Event->RECEIVE.BufferCount; i++) {
            const QUIC_BUFFER* buffer = &Event->RECEIVE.Buffers[i];
            // logic here - hand off to a processing queue
            printf("Received %u bytes\n", buffer->Length);
        }
        break;

    case QUIC_STREAM_EVENT_SEND_COMPLETE:
        // Clean up resources linked to a completed send if necessary
        break;

    case QUIC_STREAM_EVENT_SHUTDOWN_COMPLETE:
        // Stream is dead. Close the handle.
        MsQuic->StreamClose(Stream);
        break;

    default: break;
    }
    return QUIC_STATUS_SUCCESS;
}

//The Connection Callback: Manages the lifecycle of a single client.
QUIC_STATUS QUIC_API ServerConnectionCallback(HQUIC Connection, void* Context, QUIC_CONNECTION_EVENT* Event) {
    switch (Event->Type) {
    case QUIC_CONNECTION_EVENT_CONNECTED:
        printf("[conn] Client connected\n");
        break;

    case QUIC_CONNECTION_EVENT_PEER_STREAM_STARTED:
        // A client opened a new data stream.
        // We attach our Stream Callback to this specific stream.
        MsQuic->SetCallbackHandler(Event->PEER_STREAM_STARTED.Stream, (void*)ServerStreamCallback, NULL);
        break;

    case QUIC_CONNECTION_EVENT_SHUTDOWN_COMPLETE:
        printf("[conn] Client disconnected\n");
        MsQuic->ConnectionClose(Connection);
        break;

    default: break;
    }
    return QUIC_STATUS_SUCCESS;
}

//Listener Callback: The gatekeeper for the UDP port.
QUIC_STATUS QUIC_API ServerListenerCallback(HQUIC Listener, void* Context, QUIC_LISTENER_EVENT* Event) {
    switch (Event->Type) {
    case QUIC_LISTENER_EVENT_NEW_CONNECTION:
        // Apply the server configuration to the new connection.
        // This is where TLS is negotiated.
        MsQuic->ConnectionSetConfiguration(Event->NEW_CONNECTION.Connection, GlobalConfiguration);

        // Route this connection to our Connection Callback handler.
        MsQuic->SetCallbackHandler(Event->NEW_CONNECTION.Connection, (void*)ServerConnectionCallback, NULL);
        break;

    default: break;
    }
    return QUIC_STATUS_SUCCESS;
}

NATIVE_API void run_c_server(void) {
    QUIC_STATUS Status = QUIC_STATUS_SUCCESS;
    HQUIC Registration = NULL;
    HQUIC Listener = NULL;

    //Open API
    if (QUIC_FAILED(Status = MsQuicOpenVersion(2, &MsQuic))) return;

    //Using MAX_THROUGHPUT ensures MSQUIC spins up enough worker threads for your CPU cores.
    QUIC_REGISTRATION_CONFIG RegConfig = { "scalable_server", QUIC_EXECUTION_PROFILE_MAX_THROUGHPUT };
    if (QUIC_FAILED(Status = MsQuic->RegistrationOpen(&RegConfig, &Registration))) goto Error;

    //Global Configuration (Settings)
    QUIC_SETTINGS Settings = { 0 };
    Settings.IdleTimeoutMs = 60000;
    Settings.IsSet.IdleTimeoutMs = TRUE;
    Settings.PeerBidiStreamCount = 100; // Allow 100 parallel streams per client
    Settings.IsSet.PeerBidiStreamCount = TRUE;

    if (QUIC_FAILED(Status = MsQuic->ConfigurationOpen(Registration, NULL, 0, &Settings, sizeof(Settings), NULL, &GlobalConfiguration))) goto Error;

    // Credentials (THE TRICKY PART) - llm
    // For a real server, you MUST provide a certificate. 
    // This example uses 'Schannel' dummy flags for a self-signed/test environment.
    QUIC_CREDENTIAL_CONFIG CredConfig = { 0 };
    CredConfig.Flags = QUIC_CREDENTIAL_FLAG_NONE;
    CredConfig.Type = QUIC_CREDENTIAL_TYPE_NONE; // On Windows, this may require specific store setup.

    if (QUIC_FAILED(Status = MsQuic->ConfigurationLoadCredential(GlobalConfiguration, &CredConfig))) goto Error;

    //Start Listener
    if (QUIC_FAILED(Status = MsQuic->ListenerOpen(Registration, ServerListenerCallback, NULL, &Listener))) goto Error;

    QUIC_ADDR Address = { 0 };
    QuicAddrSetFamily(&Address, QUIC_ADDRESS_FAMILY_UNSPEC); // Dual-stack IPv4/IPv6
    QuicAddrSetPort(&Address, 443);

    const uint8_t alpn[] = { 4, 's', 'e', 'r', 'v' }; // ALPN: "serv"
    QUIC_BUFFER AlpnBuffer = { sizeof(alpn), (uint8_t*)alpn };

    if (QUIC_FAILED(Status = MsQuic->ListenerStart(Listener, &AlpnBuffer, 1, &Address))) goto Error;

    printf("Scalable QUIC Server Listening on Port 443...\n");
    return;

Error:
    if (Listener) MsQuic->ListenerClose(Listener);
    if (GlobalConfiguration) MsQuic->ConfigurationClose(GlobalConfiguration);
    if (Registration) MsQuic->RegistrationClose(Registration);
    MsQuicClose(MsQuic);
}