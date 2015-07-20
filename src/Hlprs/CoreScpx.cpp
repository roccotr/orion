#include "CoreScpl.hpp"

using namespace orion;

	const int CoreScpl::iCconnectattempt = 1;

  __fastcall CoreScpl::CoreScpl (int iDefaultbuffersize) {}
  __fastcall CoreScpl::~CoreScpl () {}

	/*INIT*/
	void __fastcall CoreScpl::init_ (AnsiString sPipename) {
	}

	/*CoNNeCT*/
	bool __fastcall CoreScpl::cnnct (int iPort) {
		return false;
	}

	/*LISTeN*/
	bool __fastcall CoreScpl::listn (int iPort, int iNumports) {
		return false;
	}

	/*ACCePT*/
	AnsiString __fastcall CoreScpl::accpt () {
		return NULL;
	}

	/*SEND*/
	int __fastcall CoreScpl::send_ (	const char* bBuffer, int iLength) {
		return -1;
	}

	/*RECV*/
	int __fastcall CoreScpl::recv_ (	char* bBuffer, int iLength) {
		return -1;
	}

	bool __fastcall CoreScpl::iscnn () {
		return false;
	}
