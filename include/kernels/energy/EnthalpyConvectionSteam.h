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

  
    MaterialProperty<RealGradient> & _darcy_mass_flux_steam;
    MaterialProperty<RealGradient> & _Ddarcy_mass_flux_steamDH;
    //VariableGradient & _grad_enthalpy_steam;
    VariableValue & _enthalpy_steam;
    VariableValue & _denthalpy_steamdH_P;
};
#endif //ENTHALPYCONVECTIONSTEAM
