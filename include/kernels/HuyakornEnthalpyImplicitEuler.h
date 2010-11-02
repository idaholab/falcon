
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

  HuyakornEnthalpyImplicitEuler(const std::string & name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  
  MaterialProperty<Real> & _temp;
  MaterialProperty<Real> & _temp_old;
  MaterialProperty<Real> & _rho;
  MaterialProperty<Real> & _rho_old;

  MaterialProperty<Real> & _Heat;
  MaterialProperty<Real> & _Heat_old;

  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _rho_r;
  MaterialProperty<Real> & _cp_r;

};
#endif //HUYAKORNENTHALPYIMPLICITEULER
