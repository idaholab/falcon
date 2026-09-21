#include "FalconPostprocessorFunction.h"

registerMooseObject("FalconApp", FalconPostprocessorFunction);

InputParameters
FalconPostprocessorFunction::validParams()
{
  InputParameters params = Function::validParams();
  params.addRequiredParam<PostprocessorName>(
      "pp", "The postprocessor whose value this function returns.");
  params.addClassDescription(
      "A Function that simply returns the current value of a named Postprocessor, for feeding a "
      "computed scalar into any parameter typed FunctionName (eg PorousFlowPeacemanBorehole's "
      "'bottom_p_or_t').");
  return params;
}

FalconPostprocessorFunction::FalconPostprocessorFunction(const InputParameters & parameters)
  : Function(parameters), _pp(getPostprocessorValue("pp"))
{
}

Real
FalconPostprocessorFunction::value(Real /*t*/, const Point & /*p*/) const
{
  return _pp;
}
