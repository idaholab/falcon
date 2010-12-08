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

  EnthalpyConvectionSteam(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();  

  
  MaterialProperty<RealGradient> &_darcy_flux_s;
  MaterialProperty<RealGradient> &_GHs;
  MaterialProperty<Real> &_rho_s;
  
};
#endif //ENTHALPYCONVECTIONSTEAM
