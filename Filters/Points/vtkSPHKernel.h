/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSPHKernel.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSPHKernel - a family of SPH interpolation kernels

// .SECTION Description
// vtkSPHKernel is an abstract superclass for smooth particle hydrodynamics
// interpolation kernels as described by D.J. Price (see full reference
// below).
//
// Note that the kernel operates over a volume in space defined by a radius
// at a sampling point. The kernel implicitly assumes that the particles
// making up the input data satisfies physical properties such as
// conservation of mass. Therefore subclasses of this kernel are not
// generally applicable for interpolation processes, and operate in
// conjunction with the vthSPHInterpolator class.
//
// By default the kernel computes local particle volume from the spatial step^3.
// However, if both an optional mass and density arrays are provided then they are
// used to compute local volume.
//

// .SECTION Caveats
// See D.J. Price, Smoothed particle hydrodynamics and magnetohydrodynamics,
// J. Comput. Phys. 231:759-794, 2012. Especially equation 49.

// .SECTION See Also
// vtkSPHKernel vtkSPHQuinticKernel vtkInterpolationKernel vtkGaussianKernel
// vtkShepardKernel vtkLinearKernel


#ifndef vtkSPHKernel_h
#define vtkSPHKernel_h

#include "vtkFiltersPointsModule.h" // For export macro
#include "vtkInterpolationKernel.h"
#include "vtkStdString.h" // For vtkStdString ivars

class vtkIdList;
class vtkDoubleArray;
class vtkDataArray;
class vtkFloatArray;


class VTKFILTERSPOINTS_EXPORT vtkSPHKernel : public vtkInterpolationKernel
{
public:
  // Description:
  // Standard methods for instantiation, obtaining type information, and printing.
  vtkTypeMacro(vtkSPHKernel,vtkInterpolationKernel);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // The user defined intial particle spatial step.
  vtkSetClampMacro(SpatialStep,double,0.0,VTK_FLOAT_MAX);
  vtkGetMacro(SpatialStep,double);

  // Description:
  // The domain dimension, default to 3.
  vtkSetClampMacro(Dimension,int,1,3);
  vtkGetMacro(Dimension,int);

  // Description:
  // Specify the density array.
  virtual void SetDensityArray(vtkDataArray*);
  vtkGetObjectMacro(DensityArray,vtkDataArray);

  // Description:
  // Specify the mass array.
  virtual void SetMassArray(vtkDataArray*);
  vtkGetObjectMacro(MassArray,vtkDataArray);

  // Description:
  // Produce the computational parameters for the kernel. Invoke this method
  // after setting initial values like SpatialStep.
  virtual void Initialize(vtkAbstractPointLocator *loc, vtkDataSet *ds,
                          vtkPointData *pd);

  // Description:
  // Given a point x, determine the points around x which form an
  // interpolation basis. The user must provide the vtkIdList pids, which will
  // be dynamically resized as necessary. The method returns the number of
  // points in the basis. Typically this method is called before
  // ComputeWeights().
  virtual vtkIdType ComputeBasis(double x[3], vtkIdList *pIds);

  // Description:
  // Given a point x, and a list of basis points pIds, compute interpolation
  // weights, plus derivative weights, associated with these basis points.
  virtual vtkIdType ComputeGradWeights(double x[3], vtkIdList *pIds,
                                       vtkDoubleArray *weights,
                                       vtkDoubleArray *gradWeights) = 0;

protected:
  vtkSPHKernel();
  ~vtkSPHKernel();

  // Instance variables
  double SpatialStep; //also known as spatial step
  int Dimension;
  bool RequiresInitialization;

  // Optional arrays aid in the interpolation process
  vtkDataArray *DensityArray;
  vtkDataArray *MassArray;

  // Internal data members generated by DefineKernelParameters
  double CutoffFactor; //varies across each kernel, typically=3.0
  double Cutoff; //the smoothing length * cutoff factor
  double FacW; // spline coefficient
  double GradW; // spline coefficient (derived quantities)
  double NormDist; //normalization factor 1/smoothing length
  double DefaultVolume; //if mass and density arrays not specified, use this
  bool UseArraysForVolume;

private:
  vtkSPHKernel(const vtkSPHKernel&);  // Not implemented.
  void operator=(const vtkSPHKernel&);  // Not implemented.
};

#endif
