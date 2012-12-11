#ifndef UTILS_CONFIG_H
#define UTILS_CONFIG_H

/* Cmake will define MyLibrary_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define MyLibrary_EXPORTS when
building a DLL on windows.
*/
// We are using the Visual Studio Compiler and building Shared libraries

#if defined (_WIN32)
  #if defined(imcore_EXPORTS)
    #define  UTILS_EXPORT __declspec(dllexport)
  #else
    #define  UTILS_EXPORT //__declspec(dllimport)
  #endif /* platereader_EXPORTS */
#else /* defined (_WIN32) */
  #define UTILS_EXPORT
#endif

#ifdef USE_RESTRICT_POINTER
#	define RESTRICT restrict
#	define _RESTRICT_ __restrict__
#else
#	define RESTRICT
#	define _RESTRICT_
#endif

#endif // UTILS_CONFIG_H
