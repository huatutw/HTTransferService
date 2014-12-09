#if !defined(_ADBCDLL_)
#define _ADBCDLL_

//#pragma comment(linker, "/NODEFAULTLIB:libc.lib") 
//#pragma comment(lib, "AdbcS.lib")

/*
#pragma message("Automatically linking with Adbc.dll") 

#ifdef ADBC_EXPORTS
#define ADBC_API __declspec(dllexport)
#else
#define ADBC_API __declspec(dllimport)
#endif
*/
#import <C:\\Program Files\\Common Files\\system\\ole db\\Oledb32.dll> raw_interfaces_only exclude("IDBPromptInitialize", "IDataInitialize") no_auto_exclude
#import <C:\\Program Files\\Common Files\\system\\ado\\Msado15.dll> rename("EOF", "adoEOF") rename("BOF", "adoBOF")
#import <C:\\Program Files\\Common Files\\system\\ado\\msadox.dll>

#include <map>
#include <vector>
using namespace std;

#include <icrsint.h>

#include "AdbcCommDef.h"
#include "AString.h"
#include "ADateTime.h"

#include "AVariantConv.h"

#include "ABlob.h"

//#include "AExtDataMgr.h"
#include "AException.h"
#include "AConnect.h"
#include "ACommand.h"
#include "ARecordset.h"

#include "ACommFun.h"
#include "AError.h"
#include "AErrors.h"
#include "AParameter.h"
#include "AParameters.h"

#include "AField.h"


#endif
