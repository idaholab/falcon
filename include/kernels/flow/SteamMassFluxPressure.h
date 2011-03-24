#ifndef STEAMMASSFLUXPRESSURE
#define STEAMMASSFLUXPRESSURE

#include "Diffusion.h"
#include "Material.h"

//Forward Declarations
class SteamMassFluxPressure;

template<>
InputParameters validParams<SteamMassFluxPressure>();

class SteamMassFluxPressure : public Diffusion
{
public:

  SteamMassFluxPressure(const std::string & name, InputParameters parameters);
    
protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  MaterialProperty<Real> &_tau_steam;
};
#endif //STEAMMASSFLUXPRESSURE
