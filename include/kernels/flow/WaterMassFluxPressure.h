#ifndef WATERMASSFLUXPRESSURE
#define WATERMASSFLUXPRESSURE

#include "Diffusion.h"
#include "Material.h"

//Forward Declarations
class WaterMassFluxPressure;

template<>
InputParameters validParams<WaterMassFluxPressure>();

class WaterMassFluxPressure : public Diffusion
{
public:

  WaterMassFluxPressure(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  
  unsigned int _h_var;
  VariableGradient & _grad_p;
  MaterialProperty<Real> & _Dtau_waterDP;
  MaterialProperty<Real> & _Dtau_waterDH;
  MaterialProperty<Real> & _tau_water;
  MaterialProperty<RealGradient> &   _darcy_mass_flux_water;
};
#endif //WATERMASSFLUXPRESSURE
