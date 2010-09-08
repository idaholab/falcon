#include "ImplicitBackwardDifference2.h"

#ifndef DARCYIMPLICITBACKWARDDIFFERENCE2
#define DARCYIMPLICITBACKWARDDIFFERENCE2

//Forward Declarations
class DarcyImplicitBackwardDifference2;

template<>
InputParameters validParams<DarcyImplicitBackwardDifference2>();

class DarcyImplicitBackwardDifference2 : public ImplicitBackwardDifference2
{
public:

  DarcyImplicitBackwardDifference2(const std::string & name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> & _c_f;
  MaterialProperty<Real> & _porosity;
  MaterialProperty<Real> & _rho_w;
  
};
#endif //DARCYIMPLICITBACKWARDDIFFERENCE2
