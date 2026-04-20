#include <msquic.h>
// this specific implementation is very opinionated and rigid and its life cycle needs to be studied
// the main all var is QUIC_API_TABLE contents: goddamn am not ready for that many abstractions

void startserver(QUIC_API_TABLE qapi) {
	QUIC_STATUS Status = QUIC_STATUS_SUCCESS;// enum storage for sts checking
	//the main handler accepts the main var and will start on that
	if (QUIC_FAILED(Status = MsQuicOpen2(qapi)) {// alternative was succeded too
		// used an wrapped macro over the actual func call to auto mitigate to ver 2
		perror("MsQuicOpen2"); return;
	}
	// now almost all the work is done by alone this func call - thats the issue huge abstraction
	// step by step: is the call of the macroed func call then immediate is the msquiclibraryload
	// it is misleading by name, typical microslop. the flow happens this way:

}