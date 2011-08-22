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

  EnthalpyConvectionWater(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
  MaterialProperty<Real> & _Dtau_waterDH;
  MaterialProperty<Real> & _Dtau_waterDP;
  MaterialProperty<RealGradient> & _darcy_mass_flux_water;
  MaterialProperty<Real> & _tau_water;
  VariableValue  & _enthalpy_water;
  VariableValue & _denthalpy_waterdH_P;
  VariableValue & _denthalpy_waterdP_H;
  unsigned int  _p_var;
  VariableGradient & _grad_p;
  

};
#endif //ENTHALPYCONVECTIONWATER
