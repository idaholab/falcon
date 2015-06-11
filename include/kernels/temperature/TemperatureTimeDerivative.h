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

#ifndef TEMPERATURETIMEDERIVATIVE
#define TEMPERATURETIMEDERIVATIVE

//Forward Declarations
class TemperatureTimeDerivative;

template<>
InputParameters validParams<TemperatureTimeDerivative>();

class TemperatureTimeDerivative : public TimeDerivative
{
public:

  TemperatureTimeDerivative(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

    const MaterialProperty<Real> & _density_water;
    const MaterialProperty<Real> & _density_water_old;

    bool _has_chem_reactions;
    const MaterialProperty<Real> & _porosity;
    const MaterialProperty<Real> * _porosity_old;

    const MaterialProperty<Real> & _specific_heat_water;
    const MaterialProperty<Real> & _specific_heat_rock;
    const MaterialProperty<Real> & _density_rock;

    VariableValue & _u_old;
};
#endif //TEMPERATURETIMEDERIVATIVE
