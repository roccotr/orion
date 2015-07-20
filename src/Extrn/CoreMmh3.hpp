#ifndef COREMMH3
#define COREMMH3
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#ifdef VSTUDIO
	//#include "pstdint.h"
#else
	#include <stdint.h>
#endif

void bmix32 ( uint32_t & h1, uint32_t & h2, uint32_t & h3, uint32_t & h4,
              uint32_t & k1, uint32_t & k2, uint32_t & k3, uint32_t & k4,
              uint32_t & c1, uint32_t & c2 );

/*Austin Appleby @ Google*/
void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );
void MurmurHash3_x86_64  ( const void * key, int len, uint32_t seed, void * out );
void MurmurHash3_x86_128 ( const void * key, int len, uint32_t seed, void * out );

void MurmurHash3_x64_32  ( const void * key, int len, uint32_t seed, void * out );
void MurmurHash3_x64_64  ( const void * key, int len, uint32_t seed, void * out );
void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );


#endif