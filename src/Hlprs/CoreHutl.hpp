#ifndef COREHUTL
#define COREHUTL
#include <boost/filesystem.hpp> 
#include "CoreVcl_.hpp"
#include "CoreList.hpp"

namespace orion {

class CoreHutl
{
private:

	static l64 iVglobaltimestamppartial;

	bool static match (char *bString, char *bWild);
  bool static __fastcall getst (AnsiString sFilename,SYSTEMTIME* cWrite);	

public:

	void static __fastcall readr (	AnsiString sPath, AnsiString sMask,
																	AnsiString** sFilenames, int& iCount,
																	AnsiString sFoldermask = NULL);
	void static __fastcall readr (	AnsiString sPath, AnsiString sMask,
																	CoreList<AnsiString>& cFilenames);
	AnsiString static __fastcall appnm (void* hInstance);
	bool static __fastcall crdir (AnsiString sPath);
	bool static __fastcall ispvl (AnsiString sPath); 
	bool static __fastcall rmdir (AnsiString sPath);
	int64vcl static gettp ();

};

};

#endif