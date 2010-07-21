#ifndef HUYAKORNENTHALPYDIFFUSION
#define HUYAKORNENTHALPYDIFFUSION

#include "Diffusion.h"
#include "Material.h"

class HuyakornEnthalpyDiffusion;

template<>
InputParameters validParams<HuyakornEnthalpyDiffusion>();

class HuyakornEnthalpyDiffusion : public Diffusion
{
public:

  HuyakornEnthalpyDiffusion(std::string name, MooseSystem & moose_system, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();


  MaterialProperty<Real> &_thermal_conductivity;
  MaterialProperty<Real> &_dTbydH_P;
  
};
#endif //HUYAKORNENTHALPYDIFFUSION
