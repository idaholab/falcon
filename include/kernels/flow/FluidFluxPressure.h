#ifndef FLUIDFLUXPRESSURE
#define FLUIDFLUXPRESSURE

#include "Diffusion.h"
#include "Material.h"

//Forward Declarations
class FluidFluxPressure;

template<>
InputParameters validParams<FluidFluxPressure>();

class FluidFluxPressure : public Diffusion
{
public:
    
    FluidFluxPressure(const std::string & name, InputParameters parameters);
    
protected:
    virtual Real computeQpResidual();
    
    virtual Real computeQpJacobian();
    
    MaterialProperty<Real> & _tau_water;
    MaterialProperty<RealGradient> & _darcy_mass_flux_water;
};
#endif //FLUIDFLUXPRESSURE
