/*
 * imcore_config.h
 *
 *  Created on: Dec 10, 2012
 *      Author: julio
 */

#ifndef IMCORE_CONFIG_H_
#define IMCORE_CONFIG_H_

/* Cmake will define MyLibrary_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define MyLibrary_EXPORTS when
building a DLL on windows.
*/
// We are using the Visual Studio Compiler and building Shared libraries

#if defined (_WIN32)
  #if defined(imcore_EXPORTS)
    #define  IMCORE_EXPORT __declspec(dllexport)
  #else
    #define  IMCORE_EXPORT //__declspec(dllimport)
  #endif /* platereader_EXPORTS */
#else /* defined (_WIN32) */
  #define IMCORE_EXPORT
#endif

#ifdef USE_RESTRICT_POINTER
#	define RESTRICT restrict
#	define _RESTRICT_ __restrict__
#else
#	define RESTRICT
#	define _RESTRICT_
#endif

#endif /* IMCORE_CONFIG_H_ */
