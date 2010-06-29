#ifndef ENTHALPYCONVECTIONWATER
#define ENTHALPYCONVECTIONWATER

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class EnthalpyConvectionWater;

template<>
InputParameters validParams<EnthalpyConvectionWater>();

class EnthalpyConvectionWater : public Kernel
{
public:

  EnthalpyConvectionWater(std::string name, MooseSystem & moose_system, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  MooseArray<Real> & _pressure;
  MooseArray<RealGradient> & _grad_p;
  MooseArray<RealGradient> &_darcy_flux_w;
  MooseArray<Real> &_rho_w;

  
};
#endif //ENTHALPYCONVECTIONWATER
