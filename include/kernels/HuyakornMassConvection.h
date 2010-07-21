#ifndef HUYAKORNMASSCONVECTION
#define HUYAKORNMASSCONVECTION

#include "Diffusion.h"
#include "Material.h"

//Forward Declarations
class HuyakornMassConvection;

template<>
InputParameters validParams<HuyakornMassConvection>();

class HuyakornMassConvection : public Diffusion
{
public:

  HuyakornMassConvection(std::string name, MooseSystem & moose_system, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> &_darcy_params_w;
  MaterialProperty<Real> &_darcy_params_s;

  
};
#endif //HUYAKORNMASSCONVECTION
