#ifdef  _WINDOWS_SOURCE
#ifdef  __TRACmdAddinMod
#define ExportedByTRACmdAddinMod     __declspec(dllexport)
#else
#define ExportedByTRACmdAddinMod     __declspec(dllimport)
#endif
#else
#define ExportedByTRACmdAddinMod
#endif
