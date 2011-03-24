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

  MaterialProperty<Real> &_tau_steam;
  MaterialProperty<Real> &_gravity;
  MaterialProperty<Real> &_enthalpy_saturated_steam;
  MaterialProperty<Real> &_density_steam;
  
  MaterialProperty<RealVectorValue> &_gravity_vector;
};
#endif //ENTHALPYCONVECTIONZSTEAM
