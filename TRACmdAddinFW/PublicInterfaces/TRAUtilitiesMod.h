#ifdef  _WINDOWS_SOURCE
#ifdef  __TRAUtilitiesMod
#define ExportedByTRAUtilitiesMod     __declspec(dllexport)
#else
#define ExportedByTRAUtilitiesMod     __declspec(dllimport)
#endif
#else
#define ExportedByTRAUtilitiesMod
#endif
