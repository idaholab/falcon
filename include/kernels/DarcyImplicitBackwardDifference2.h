#include "ImplicitBackwardDifference2.h"

#ifndef DARCYIMPLICITBACKWARDDIFFERENCE2
#define DARCYIMPLICITBACKWARDDIFFERENCE2



class DarcyImplicitBackwardDifference2 : public ImplicitBackwardDifference2
{
public:

  DarcyImplicitBackwardDifference2(std::string name,
                     InputParameters parameters,
                     std::string var_name,
                     std::vector<std::string> coupled_to,
                     std::vector<std::string> coupled_as);

  virtual void subdomainSetup();

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  std::vector<Real> * _c_f;
  std::vector<Real> * _porosity;
  std::vector<Real> * _rho_w;
  
};
#endif //DARCYIMPLICITBACKWARDDIFFERENCE2
