/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkTkInternals.h.in,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkTkInternals_h
#define __vtkTkInternals_h

#define HAVE_LIMITS_H
/* #undef HAVE_UNISTD_H */

// This widget requires access to structures that are normally 
// not visible to Tcl/Tk applications. For this reason you must
// have access to tkInt.h
// #include "tkInt.h"
#ifdef _WIN32
extern "C"
{
#include "tkWinInt.h" 
}
#endif

#ifdef VTK_USE_CARBON
// Tk #defines Status in:
// Tk.framework/Versions/8.4/Headers/X11/Xlib.h,
// which conflicts with a member of the FPUInformationIntel
// struct in Apple's new headers for i386:
// CarbonCore.framework/Versions/A/Headers/MachineExceptions.h
#if defined (i386)
#undef Status
#endif /* defined (i386) */
#include "tkMacOSXInt.h"
#endif

#endif /* __vtkTkInternals_h */
 