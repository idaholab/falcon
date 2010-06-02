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

  DarcyImplicitBackwardDifference2(std::string name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MooseArray<Real> & _c_f;
  MooseArray<Real> & _porosity;
  MooseArray<Real> & _rho_w;
  
};
#endif //DARCYIMPLICITBACKWARDDIFFERENCE2
