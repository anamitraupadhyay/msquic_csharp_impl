#include <msquic.h>
// this specific implementation is very opinionated and rigid and its life cycle needs to be studied
// the main all var is QUIC_API_TABLE contents: goddamn am not ready for that many abstractions

void startserver(QUIC_API_TABLE qapi, /*Registration*/HQUIC rhandle) {
	QUIC_STATUS Status = QUIC_STATUS_SUCCESS;// enum storage for sts checking
	//the main handler accepts the main var and will start on that
	if (QUIC_FAILED(Status = MsQuicOpen2(qapi)) {// alternative was succeded too
		// used an wrapped macro over the actual func call to auto mitigate to ver 2
		perror("MsQuicOpen2"); return;
	}// already those funcs are already set, so the msquic->reg likes of those can be called
	// now almost all the work is done by alone this func call - thats the issue huge abstraction
	// step by step: is the call of the macroed func call then immediate is the msquiclibraryload
	// it is misleading by name, typical microslop. the flow happens this way:
	if(QUIC_FAILED(Status = qapi->RegistrationOpen(rhandle)){// the handle might be incomplete params i mean, though the current handle is pointing to the
		// the funct ptrs already inited and its currently is MsQuicRegistrationOpen
		perror("registran"); return;
	}
}
//the another question to be asked is to confirm the function signatures am doing all the steps void and all are required or not
// as i have seen the weirdest QUIC_MAIN_EXPORT macro added to the sample main calling the funcs
void serverconfig(QUIC_API_TABLE qapi, HQUIC config){
	// the server configuration maybe name and all the registrations
	QUIC_STATUS status = QUIC_STATUS_SUCCESS;
	if(QUIC_FAILED(status = qapi->ConfigurationOpen(config))){
		// similar case as in registrationopen the func will overtake this and do and the inderlying might change
	}
}

int main(){
	QUIC_API_TABLE qapi = NULL;
	//Registration rhandle = NULL; wrong took a guess but it was wrong for all quic_handle_types the HQUIC that has void *context and enum handle type
	HQUIC reg, config, listener, stream, connection;//3 or 4 more to add: here only the registration, config, listener and stream are constants
	// though for clients listener is not required and there is also need for connection if am not wrong: though i percieve it will be later decided its
	// server or client, here offcourse server with listener property
	startserver(qapi, reg);// populates the api table for all steps and inits the reqistration
	// similar steps are required for the rest of the models passing of the qapi and managing them if required and the handle hquic respectively
	// todo : signatures to be applied
	//while(1)}
		// what live here ok maybe not while loop its just async firebacks of the tagged union
	//} is main even required? since the model is async theres high chance it may not
	serverconfig(qapi, config);
}
