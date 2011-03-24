
#include "ImplicitEuler.h"

#ifndef HUYAKORNENTHALPYIMPLICITEULER
#define HUYAKORNENTHALPYIMPLICITEULER

//Forward Declarations
class HuyakornEnthalpyImplicitEuler;

template<>
InputParameters validParams<HuyakornEnthalpyImplicitEuler>();

class HuyakornEnthalpyImplicitEuler : public ImplicitEuler
{
public:

  HuyakornEnthalpyImplicitEuler(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  
  MaterialProperty<Real> & _temp;
  MaterialProperty<Real> & _temp_old;
  MaterialProperty<Real> & _density;
  MaterialProperty<Real> & _density_old;

  MaterialProperty<Real> & _Heat;
  MaterialProperty<Real> & _Heat_old;

  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _density_rock;
  MaterialProperty<Real> & _specific_heat_rock;

};
#endif //HUYAKORNENTHALPYIMPLICITEULER
