/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkBlankStructuredGridWithImage.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkBlankStructuredGridWithImage - blank a structured grid with an image
// .SECTION Description
// This filter can be used to set the blanking in a structured grid with 
// an image. The filter takes two inputs: the structured grid to blank, 
// and the image used to set the blanking. Make sure that the dimensions of
// both the image and the structured grid are identical.
//
// Note that the image is interpreted as follows: zero values indicate that
// the structured grid point is blanked; non-zero values indicate that the
// structured grid point is visible. The blanking data must be unsigned char.

// .SECTION See Also
// vtkStructuredGrid

#ifndef __vtkBlankStructuredGridWithImage_h
#define __vtkBlankStructuredGridWithImage_h

#include "vtkStructuredGridAlgorithm.h"

class vtkImageData;

class VTK_GRAPHICS_EXPORT vtkBlankStructuredGridWithImage : public vtkStructuredGridAlgorithm
{
public:
  static vtkBlankStructuredGridWithImage *New();
  vtkTypeRevisionMacro(vtkBlankStructuredGridWithImage,vtkStructuredGridAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set / get the input image used to perform the blanking.
  void SetBlankingInput(vtkImageData *input);
  vtkImageData *GetBlankingInput();

protected:
  vtkBlankStructuredGridWithImage();
  ~vtkBlankStructuredGridWithImage();

  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  virtual int FillInputPortInformation(int port, vtkInformation *info);

private:
  vtkBlankStructuredGridWithImage(const vtkBlankStructuredGridWithImage&);  // Not implemented.
  void operator=(const vtkBlankStructuredGridWithImage&);  // Not implemented.
};

#endif
