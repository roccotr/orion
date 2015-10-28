#ifdef ORDBB_EXPORTS
#define ORDBB_API __declspec(dllexport)
#else
#define ORDBB_API __declspec(dllimport)
#endif

extern "C" ORDBB_API void* new__ (char* cAddress, int iPort, int iWorkers, int iBulksize);
extern "C" ORDBB_API bool delte (void* cHandlefactory);
extern "C" ORDBB_API void* addrf (void* cHandlefactory);
extern "C" ORDBB_API bool subrf (	void* cHandleclient);
extern "C" ORDBB_API void* runpb (void* cHandleclient, bool bOptimistic, int iInputlength, void* cInputdata, int& iOutputlength);
extern "C" ORDBB_API void* runth (void* cHandleclient, bool bOptimistic, int iInputlength, void* cInputdata, int& iOutputlength);