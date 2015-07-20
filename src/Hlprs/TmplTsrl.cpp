#include "CoreTsrl.hpp" 

using namespace orion;

	/*SeRiaLiZe*/
	template<class T>
	int __fastcall CoreTsrl::srlze (T& cThrift, bool bHeader) {
		int iVreturn;

		iVreturn = cThrift.write (cVprotocol.get ());
		if (bHeader) return iVreturn + sizeof (int);
		else return iVreturn;
	}

	template<class T>	
	int __fastcall CoreTsrl::parse (T& cThrift) {

		return cThrift.read (cVprotocol.get ()); 
	}