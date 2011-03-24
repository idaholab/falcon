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

  
  MaterialProperty<RealGradient> &_darcy_flux_steam;
  MaterialProperty<RealGradient> &_Genthalpy_saturated_steam;
  MaterialProperty<Real> &_density_steam;
  
};
#endif //ENTHALPYCONVECTIONSTEAM
