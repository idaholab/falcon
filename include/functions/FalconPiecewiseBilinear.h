//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Function.h"

template <typename>
class ColumnMajorMatrixTempl;
typedef ColumnMajorMatrixTempl<Real> ColumnMajorMatrix;
class BilinearInterpolation;

/**
 * FalconPiecewiseBilinear reads from a file the information necessary to build the vectors x and y and
 * the ColumnMajorMatrix z, and then sends those (along with a sample point) to
 * BilinearInterpolation.
 * See BilinearInterpolation in moose/src/utils for a description of how that works...it is a 2D
 * linear
 * interpolation algorithm.  The format of the data file must be the following:
 *
 * 1,2
 * 1,1,2
 * 2,3,4
 *
 * The first row is the x vector data.
 * After the first row, the first column is the y vector data.
 * The rest of the data is used to build the ColumnMajorMatrix z.
 *
 * x = [1 2]
 * y = [1 2]
 *
 * z = [1 2]
 *     [3 4]
 *
 *     z has to be x.size() by y.size()
 *
 * PiecewisBilinear also sends samples to BilinearInterpolation.  These samples are the z-coordinate
 * of the current
 * integration point, and the current value of time.  The name of the file that contains this data
 * has to be included
 * in the function block of the inpute file like this...data_file = example.csv.
 */
class FalconPiecewiseBilinear : public Function
{
public:
  static InputParameters validParams();

  FalconPiecewiseBilinear(const InputParameters & parameters);

  // Necessary for using forward declaration of BilinearInterpolation in std::unique_ptr
  virtual ~FalconPiecewiseBilinear();

  using Function::value;
  /**
   * This function will return a value based on the first input argument only.
   */
  virtual Real value(Real t, const Point & pt) const override;
  virtual ADReal value(const ADReal & t, const ADPoint & pt) const override;

private:
  std::unique_ptr<BilinearInterpolation> _bilinear_interp;
  const std::string _data_file_name;
  const int _axis;
  const int _yaxis;
  const int _xaxis;
  const bool _axisValid;
  const bool _yaxisValid;
  const bool _xaxisValid;
  const Real _scale_factor;
  const bool _radial;

  template <typename T, typename P>
  T valueInternal(T t, const P & p) const;

  void parse(std::vector<Real> & x, std::vector<Real> & y, ColumnMajorMatrix & z);
};
