#ifndef ENTHALPYCONVECTIONZSTEAM
#define ENTHALPYCONVECTIONZSTEAM

#include "Kernel.h"

//Forward Declarations
class EnthalpyConvectionZSteam;

template<>
InputParameters validParams<EnthalpyConvectionZSteam>();

class EnthalpyConvectionZSteam : public Kernel
{
public:

  EnthalpyConvectionZSteam(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();

  MaterialProperty<Real> &_darcy_params_s;
  MaterialProperty<Real> &_gravity;
  MaterialProperty<Real> &_Hs;
  MaterialProperty<Real> &_rho_s;
  
  MaterialProperty<RealVectorValue> &_gravity_vector;
};
#endif //ENTHALPYCONVECTIONZSTEAM
