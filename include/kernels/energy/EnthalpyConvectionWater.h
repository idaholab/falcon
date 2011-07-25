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
  

  MaterialProperty<RealGradient> & _darcy_mass_flux_water;
    MaterialProperty<RealGradient> & _Ddarcy_mass_flux_waterDH;
  MaterialProperty<Real> & _tau_water;
  VariableValue  & _enthalpy_water;
  VariableValue & _denthalpy_waterdH_P;
  unsigned int  _p_var;
  VariableValue & _p;
  

};
#endif //ENTHALPYCONVECTIONWATER
