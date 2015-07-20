#include "CoreRvrs.hpp"
#include "CoreEndn.hpp"

using namespace orion;

  __fastcall CoreRvrs::CoreRvrs () {

    InitEndian ();
  }

  __fastcall CoreRvrs::~CoreRvrs () {
  }

	/*ASCendent Int*/
	int __fastcall CoreRvrs::asc_i (int iLocal) {

		return BigLong (iLocal);
	}

	/*DEcreSCent Int*/
	int __fastcall CoreRvrs::desci (int iLocal) {

		return asc_i (-iLocal);
	}

	/*ASCendent Double*/
	double __fastcall CoreRvrs::asc_d (double dLocal) {

		return BigDouble (dLocal);
	}

	/*DEcreSCent Double*/
	double __fastcall CoreRvrs::descd (double dLocal) {

		return asc_d (-dLocal);
	}

	/*ASCendent Long long*/
	int64vcl __fastcall CoreRvrs::asc_l (int64vcl iLocal) {
	
		return BigInt64 (iLocal);
	}

	/*DEcreSCent Long long*/
	int64vcl __fastcall CoreRvrs::descl (int64vcl iLocal) {

		return asc_l (-iLocal);
	}

	/*Reverse DEcreScent Long lon*/
	int64vcl __fastcall CoreRvrs::rdesl (int64vcl iLocal) {

		return -(asc_l (iLocal));
	}





