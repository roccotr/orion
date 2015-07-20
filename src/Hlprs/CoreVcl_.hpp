#ifndef COREVCL_

#define COREVCL_
#ifdef _MSC_VER
		#define VSTUDIO
		#define LEGACYSUPPORT
	#endif
#ifdef LINUXOS
		#define LEGACYSUPPORT
#endif
#ifdef APPLEOS
#define LINUXOS
#define LEGACYSUPPORT
#endif
#define iCmllcalign 16
#ifdef LINUXOS
#define __InterlockedIncrement(HANDLE) __sync_fetch_and_add(HANDLE,1);
#define __InterlockedDecrement(HANDLE) __sync_add_and_fetch(HANDLE,-1)
#else
		#define __InterlockedIncrement(HANDLE) InterlockedIncrement(HANDLE)
		#define __InterlockedDecrement(HANDLE) InterlockedDecrement(HANDLE)
	#endif

#include "CoreVutl.hpp"
#include "CoreXutl.hpp"

#define factorymalloc CoreVutl::mllc8
#define factoryfree CoreVutl::free8

#ifdef LEGACYSUPPORT

#include "CoreTlst.hpp"

#define TList CoreTlst
#define TDateTime double
#define __Time CoreXutl::ttime
#define MAXDRIVE 3
#define MAXDIR 256
#define MAXFILE 256
#define MAXEXT 256
#define fnsplit CoreVutl::fnspl
#define ffFixed 0

#ifndef VSTUDIO

			#include "CoreAnsx.hpp"

			#define AnsiString CoreAnsx
			#define FloatToStrF CoreAnsx::_FloatToStrF

			#define timeGetTime CoreXutl::tgtme
			#define Sleep CoreXutl::sleep
			#define GetCurrentProcessId CoreXutl::getpi
			#define GetCurrentThreadId CoreXutl::getti
			#define GetVolumeInformation CoreVutl::getvi
			#define SYSTEMTIME double
			#define DWORD int
			#define DWORD_PTR int
			#define LARGE_INTEGER int
#else
			#include "CoreAnsx.hpp"
			#define AnsiString CoreAnsx
			#define FloatToStrF CoreAnsx::_FloatToStrF
		
			#include <windows.h>
#endif

#ifdef LINUXOS

			#include <stdlib.h>
			#include <string.h>
			#include <stdio.h>
			#include <inttypes.h>

			#include <unistd.h>
			#include <sys/socket.h>
			#include <netinet/ip.h>
			#include <arpa/inet.h>

			#define __fastcall
			#define _fastcall
			#define _typename
			#define EXCEPTION AnsiString
			#define bCcarriagereturn "\n"
			#define iCintansistring uint64_t
			#ifndef iCx64
					#define l64 long
					#define i64 int
					#define u64 uint64_t
			#else
					#define l64 int64_t
					#define i64 int64_t
					#define u64 uint64_t
			#endif

			typedef int BOOL;
			typedef uint64_t *ULONG_PTR;
			typedef void *PVOID;
			typedef PVOID HANDLE;
			typedef int SOCKET;
			typedef struct _OVERLAPPED {
					ULONG_PTR Internal;
					ULONG_PTR InternalHigh;
					union {
							struct {
									DWORD Offset;
									DWORD OffsetHigh;
							};
							PVOID Pointer;
					};
					HANDLE hEvent;
			} OVERLAPPED, *LPOVERLAPPED;

			#define closesocket(s) close(s);
			#define SOCKLENT unsigned int

			#define int64vcl int64_t
			#define TThread CoreTthr
			#define TEvent CoreTevn
			#define wrSignaled CoreTevn::iCwrsignaled
			#define GetModuleHandle
			typedef bool Boolean;
			typedef bool boolean;

			#include "CoreTthx.hpp"
			#include "CoreTevx.hpp"
			#include "itoa.hpp"
			#ifdef APPLEOS
					#include <stdio.h>
					#define FTELL ftell
					#define FSEEK fseek
			#endif
			#define _rotl(x, r) ((x << r) | (x >> (32 - r)))
			#define _rotl64(x, r) ((x << r) | (x >> (64 - r)))
			#define __forceinline
			#define sCunittestpath "/home/sommoyogurt/workspace/data/oriontest"
			#define iCdisablesharedmemory

#elif defined (VSTUDIO)
			#define EXCEPTION AnsiString
			#define TThreadPriority int
			#define TThread CoreTthr
			#define TEvent CoreTevn
			#define wrSignaled WAIT_OBJECT_0
			#define tpLowest THREAD_PRIORITY_LOWEST
			#define tpLower THREAD_PRIORITY_BELOW_NORMAL
			#define tpNormal THREAD_PRIORITY_NORMAL
			#define tpHigher THREAD_PRIORITY_ABOVE_NORMAL
			#define tpHighest THREAD_PRIORITY_HIGHEST
			#define __PTR PTR
			#define __FADDP st(1), st(0)
			#define dqword oword
			#define dliNotification unsigned
			#define dliFailLoadLibrary dliFailLoadLib
			#define __pfnDliFailureHook __pfnDliFailureHook2
			#include "CoreTthv.hpp"
			#include "CoreTevv.hpp"
		  #define GLOG_NO_ABBREVIATED_SEVERITIES
			#ifndef IMAGE_SIZEOF_BASE_RELOCATION
				#define IMAGE_SIZEOF_BASE_RELOCATION (sizeof(IMAGE_BASE_RELOCATION)) 
			#endif
			#define iCintansistring int
			#define ASMMODULE
			#ifndef iCx64
				#define __fastcall
				#define l64 long
				#define i64 int
				#define u64 unsigned int	
				#define u64au unsigned int
				#define u32 unsigned int
				#define int64vcl __int64
			#else
				#define __fastcall
				#define _fastcall
				#define l64 __int64
				#define i64 __int64
				#define u64 unsigned __int64	
				#define u64au unsigned __int64
				#define u32 unsigned __int32
			#endif
			#define SOCKLENT int
			#define bCcarriagereturn "\r\n"	
			typedef int OSStatus;
			typedef bool Boolean;
			#define noErr 0
			#define FTELL _ftelli64
			#define FSEEK _fseeki64
		#endif
#endif

#define ippsMalloc_8u CoreVutl::mllc8
#define ippsFree CoreVutl::free8

#define Min(a, b) ((a<=b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? a : b)


#endif
