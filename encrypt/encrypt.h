
#include <string>
using namespace std;
#pragma warning(disable:4996)





#define _EXPORTDLL
#ifdef _EXPORTDLL
#define ENCRYPT_LIB extern"C" __declspec(dllexport)  
#define IMAGE_API __declspec(dllexport) 
#else
#define ENCRYPT_LIB extern"C"__declspec(dllimport)
#define IMAGE_API __declspec(dllimport) 
#endif


ENCRYPT_LIB void GetCpuID(string& strCpuID);
