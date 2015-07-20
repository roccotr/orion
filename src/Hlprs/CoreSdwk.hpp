#ifndef CORESDWK
#define CORESDWK
#include "boost/shared_ptr.hpp"
#include "CoreSdmq.hpp"

namespace orion {

typedef boost::shared_ptr<void> PTR_VOID;

class CoreSdwk {
	bool bValive;
public:
	
	__fastcall CoreSdwk() {bValive = true;};
  virtual __fastcall ~CoreSdwk () {};
  
	virtual void init (PTR_VOID cInstance) = 0;
  virtual void work_ () = 0;
	virtual void setqq (void* cMessagequeue) = 0;
	virtual bool alive () {return bValive;};
	void abort () {bValive = false;};
	virtual void setid (AnsiString sId) {};
};

typedef boost::shared_ptr<CoreSdwk> PTR_SEDAWORKER;

};

#endif