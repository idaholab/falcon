#pragma once

#include "Function.h"

/**
 * Returns the current value of a named Postprocessor. Useful for feeding a computed scalar
 * (eg the output of a controller-style Postprocessor) into any parameter typed FunctionName,
 * such as PorousFlowPeacemanBorehole's bottom_p_or_t.
 *
 * MOOSE's own equivalent (moose/test/include/functions/PostprocessorFunction.h) is registered
 * only under MooseTestApp, so it is not available to FALCON inputs - this is a straightforward
 * port of it into FalconApp.
 */
class FalconPostprocessorFunction : public Function
{
public:
  static InputParameters validParams();

  FalconPostprocessorFunction(const InputParameters & parameters);

  using Function::value;
  virtual Real value(Real t, const Point & p) const override;

protected:
  const PostprocessorValue & _pp;
};
