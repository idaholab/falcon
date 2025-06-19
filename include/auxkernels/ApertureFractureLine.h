#pragma once

#include "AuxKernel.h"

// add description 
class ApertureFractureLine : public AuxKernel
{
public:
  static InputParameters validParams();

  ApertureFractureLine (const InputParameters & parameters);

protected:

  virtual Real computeValue() override;
  /// add doxygen for every variable
  const Point & _x1;
  const Point & _x2;
  const Real & _aHi;
  const Real & _aLo;
  const Real & _midpoint_of_sigmoid;
  const Real & _slope_at_midpoint;
};