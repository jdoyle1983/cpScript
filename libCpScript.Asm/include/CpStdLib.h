#ifndef __CPSTDLIB_H__
#define __CPSTDLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WINDLL
#define EXPORT __declspec( dllexport )
#else
#define EXPORT
#endif

EXPORT void CpStdLib_InstallConsoleIO(void* State);
EXPORT void CpStdLib_InstallFileIO(void* State);
EXPORT void CpStdLib_InstallMath(void* State);
EXPORT void CpStdLib_InstallUtilities(void* State);

#ifdef __cplusplus
}
#endif

#endif
