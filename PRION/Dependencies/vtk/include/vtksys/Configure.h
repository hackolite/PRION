/*=========================================================================

  Program:   KWSys - Kitware System Library
  Module:    $RCSfile: Configure.h.in,v $

  Copyright (c) Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef vtksys_Configure_h
#define vtksys_Configure_h

/* If we are building a kwsys .c or .cxx file, let it use the kwsys
   namespace.  When not building a kwsys source file these macros are
   temporarily defined inside the headers that use them.  */
#if defined(KWSYS_NAMESPACE)
# define kwsys_ns(x) vtksys##x
# define kwsysEXPORT vtksys_EXPORT
#endif

/* Setup the export macro.  */
#if defined(_WIN32) && 1
# if defined(vtksys_EXPORTS)
#  define vtksys_EXPORT __declspec(dllexport)
# else
#  define vtksys_EXPORT __declspec(dllimport)
# endif
#else
# define vtksys_EXPORT
#endif

/* Enable warnings that are off by default but are useful.  */
#if !defined(vtksys_NO_WARNING_ENABLE)
# if defined(_MSC_VER)
#  pragma warning ( default : 4263 ) /* no override, call convention differs */
# endif
#endif

/* Disable warnings that are on by default but occur in valid code.  */
#if !defined(vtksys_NO_WARNING_DISABLE)
# if defined(_MSC_VER)
#  pragma warning (disable: 4097) /* typedef is synonym for class */
#  pragma warning (disable: 4127) /* conditional expression is constant */
#  pragma warning (disable: 4244) /* possible loss in conversion */
#  pragma warning (disable: 4251) /* missing DLL-interface */
#  pragma warning (disable: 4305) /* truncation from type1 to type2 */
#  pragma warning (disable: 4309) /* truncation of constant value */
#  pragma warning (disable: 4514) /* unreferenced inline function */
#  pragma warning (disable: 4706) /* assignment in conditional expression */
#  pragma warning (disable: 4710) /* function not inlined */
#  pragma warning (disable: 4786) /* identifier truncated in debug info */
# endif
#endif

/* MSVC 6.0 in release mode will warn about code it produces with its
   optimizer.  Disable the warnings specifically for this
   configuration.  Real warnings will be revealed by a debug build or
   by other compilers.  */
#if !defined(vtksys_NO_WARNING_DISABLE_BOGUS)
# if defined(_MSC_VER) && (_MSC_VER < 1300) && defined(NDEBUG)
#  pragma warning ( disable : 4701 ) /* Variable may be used uninitialized.  */
#  pragma warning ( disable : 4702 ) /* Unreachable code.  */
# endif
#endif

#endif
