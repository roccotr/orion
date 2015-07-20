#include "CoreXutl.hpp"
#include <windows.h>
#include <mmsystem.h>
#include <direct.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include "CoreDbug.hpp"
  
using namespace orion;

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

	const double CoreXutl::dVtdatetimeconst = 1.0 / (24.0 * 3600.0);

  double CoreXutl::ttime () {
		double dVreturn;
		struct timeval cVnowcount;		
		FILETIME ft;
		unsigned __int64 tmpres = 0;

		GetSystemTimeAsFileTime(&ft);
		tmpres |= ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |= ft.dwLowDateTime;
		tmpres /= 10;
		tmpres -= DELTA_EPOCH_IN_MICROSECS;
		cVnowcount.tv_sec = (long)(tmpres / 1000000UL);
		cVnowcount.tv_usec = (long)(tmpres % 1000000UL);
    dVreturn =	((double) cVnowcount.tv_usec * 0.000001 + (double) cVnowcount.tv_sec) * dVtdatetimeconst;	
    return dVreturn;		
  }

	/*DELete FiLe*/
	void CoreXutl::delfl (char* bName) {

		DeleteFile(bName);
	}

	
  /*LOG ansistring EXClusive*/
  void CoreXutl::lgexc (	char* bFilename, char* bContent, 
													bool bExclusive) {
		HANDLE cVfile;
		DWORD iV;


		cVfile = CreateFile(bFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,      
												FILE_ATTRIBUTE_NORMAL, NULL); 
		if (cVfile == INVALID_HANDLE_VALUE) {
			FILE* cVfileoldway;

			cVfileoldway = fopen (bFilename, "w");
			fprintf (cVfileoldway, "%s\n", bContent);	
			fclose (cVfileoldway);
			cVfile = NULL;
		} else {
			WriteFile(cVfile, bContent, DWORD (strlen (bContent)), &iV, NULL);
			CloseHandle(cVfile);
		}
  }	

	/*GET Processor Number*/
	int CoreXutl::getpn () {

		return GetCurrentProcessorNumber ();
	}

	/*GET Process Id*/
	unsigned int CoreXutl::getpi () {

		return GetProcessId(GetCurrentProcess());
	}


	