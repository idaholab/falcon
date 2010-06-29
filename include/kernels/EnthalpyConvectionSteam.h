#ifndef ENTHALPYCONVECTIONSTEAM
#define ENTHALPYCONVECTIONSTEAM

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class EnthalpyConvectionSteam;

template<>
InputParameters validParams<EnthalpyConvectionSteam>();

class EnthalpyConvectionSteam : public Kernel
{
public:

  EnthalpyConvectionSteam(std::string name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  
  MooseArray<Real> & _pressure;
  MooseArray<RealGradient> & _grad_p;
  MooseArray<RealGradient> &_darcy_flux_s;
  MooseArray<Real> &_rho_s;
  
};
#endif //ENTHALPYCONVECTIONSTEAM
