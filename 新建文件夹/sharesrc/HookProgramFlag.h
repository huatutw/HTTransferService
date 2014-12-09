//#define HOOKFLAG _T("#**DocGuarder %d**#")

#ifdef OUTSIZE_MODE
	#define HOOKFLAG _T("#$$DocGuarder %d$$#")
#else
	#define HOOKFLAG _T("#!!DocGuarder %d!!#")
#endif