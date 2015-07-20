#ifndef COREVUTL
#define COREVUTL

#include <glog/logging.h>

#define iCmllcalignmask iCmllcalign-1
#define iCmallocattempts 10

namespace orion {

class CoreVutl
{

public:
  static int idmmy ();
  static double ddmmy ();
	static void pdmmy (void*);
	static void* getmp (void* iPointer);
  static void*  mllc8 (int iDim);
  static void free8 (void* cPointer);
  static void* reall (void* cPointer, int iDim);
  static void getvi ( char*, int, int,
                      unsigned long* iVreturn,
                      int, int, int, int);
  static void fnspl (char* bString, char* bDrive, char* bDir, char* bFile, char* bExt);

};

};
#endif