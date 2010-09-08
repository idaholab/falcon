#ifndef ENTHALPYCONVECTIONZWATER
#define ENTHALPYCONVECTIONZWATER

#include "Kernel.h"

//Forward Declarations
class EnthalpyConvectionZWater;

template<>
InputParameters validParams<EnthalpyConvectionZWater>();

class EnthalpyConvectionZWater : public Kernel
{
public:

  EnthalpyConvectionZWater(const std::string & name, MooseSystem & moose_system, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  
  virtual Real computeQpJacobian();

  MaterialProperty<Real> &_darcy_params_w;
  MaterialProperty<Real> &_gravity;
  MaterialProperty<Real> &_rho_w;
  MaterialProperty<Real> &_Hw;
  
  MaterialProperty<RealVectorValue> &_gravity_vector;
};
#endif //ENTHALPYCONVECTIONZWATER
