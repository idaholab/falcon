/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "TimeDerivative.h"

#ifndef TEMPERATURETIMEDERIVATIVEFLUID
#define TEMPERATURETIMEDERIVATIVEFLUID

//Forward Declarations
class TemperatureTimeDerivativeFluid;

template<>
InputParameters validParams<TemperatureTimeDerivativeFluid>();

class TemperatureTimeDerivativeFluid : public TimeDerivative
{
public:

  TemperatureTimeDerivativeFluid(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  VariableValue  & _density_water;
  VariableValue & _density_water_old;
  
  VariableValue  & _porosity;
  VariableValue  & _porosity_old;

  MaterialProperty<Real> & _specific_heat_water;
//  MaterialProperty<Real> & _specific_heat_rock;
//  MaterialProperty<Real> & _density_rock;
  
  VariableValue & _u_old;  
};
#endif //TEMPERATURETIMEDERIVATIVEFLUID
