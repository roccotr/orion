#include "CoreXutl.hpp"
#include "CoreVcl_.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/file.h>

#ifndef NOCOREXUTL

using namespace orion;

	const double CoreXutl::dVtdatetimeconst = 1.0 / (24.0 * 3600.0);

  /*Tdatedime TIME*/
  double CoreXutl::ttime () {
	double dVreturn;
	struct timeval cVnowcount;	
	struct timezone cVdummy;	
	
    gettimeofday (&cVnowcount, &cVdummy);
    dVreturn =	((double) cVnowcount.tv_usec * 0.000001 + (double) cVnowcount.tv_sec) * 
				dVtdatetimeconst;	
    return dVreturn;						
  }  
	
  /*TimeGetTiME*/
  double CoreXutl::tgtme () {
		double dVreturn;
		struct timeval cVnowcount;	
		struct timezone cVdummy;	
	
    gettimeofday (&cVnowcount, &cVdummy);
    dVreturn =	((double) cVnowcount.tv_usec * 0.001 + (double) cVnowcount.tv_sec * 1000.0);	
    return dVreturn;						
  }  	
  	
  /*SLEEP*/
  void CoreXutl::sleep (int iMsec) {
    usleep (iMsec * 1000);
  }
	
	/*DELete FiLe*/
	void CoreXutl::delfl (char* bName) {
		remove (bName);
	}

	/*GET Process ID*/
	unsigned int CoreXutl::getpi () {
		return getpid();
	}

	/*GET Thread Id*/
	unsigned long int CoreXutl::getti () {
		pthread_t cVthread;
		
		cVthread = pthread_self();
		return (unsigned long int) cVthread;
	}

#endif

  /*LOG ansistring EXClusive*/
#ifdef MACOS
  void CoreXutl::lgexc (	char* bFilename, char* bContent, 
													bool bExclusive) {
		int iVerror;
    FILE* cVfile;

    cVfile = fopen (bFilename, "w");
    iVerror = flock(fileno(cVfile), LOCK_EX);
    if (iVerror != -1) {
    	fprintf (cVfile, "%s\n", bContent);
    	flock(fileno(cVfile), LOCK_UN);
    }	
    fclose (cVfile);
    cVfile = NULL;
  }	
#else
 void CoreXutl::lgexc (	char* bFilename, char* bContent, 
													bool bExclusive) {
		int iVerror;
    FILE* cVfile;

    cVfile = fopen (bFilename, "w");
		fprintf (cVfile, "%s\n", bContent);	
    fclose (cVfile);
    cVfile = NULL;
  }	
#endif