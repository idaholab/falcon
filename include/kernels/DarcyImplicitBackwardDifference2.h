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

  virtual void subdomainSetup();

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  std::vector<Real> * _c_f;
  std::vector<Real> * _porosity;
  std::vector<Real> * _rho_w;
  
};
#endif //DARCYIMPLICITBACKWARDDIFFERENCE2
