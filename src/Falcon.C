#include "Moose.h"
#include "KernelFactory.h"
#include "BCFactory.h"
#include "MaterialFactory.h"
#include "AuxFactory.h"

#include "HeatConduction.h"
#include "HeatConductionImplicitEuler.h"
#include "SolidMechX.h"
#include "SolidMechY.h"
#include "SolidMechZ.h"
#include "SolidMechImplicitEuler.h"

#include "SolidMechTempCoupleX.h"
#include "SolidMechTempCoupleY.h"
#include "SolidMechTempCoupleZ.h"

#include "SolidMechPoroCoupleX.h"
#include "SolidMechPoroCoupleY.h"
#include "SolidMechPoroCoupleZ.h"

#include "SolidMechSwellingSolid.h"
#include "SolidMechSwellingGas.h"
#include "SinHeat.h"
#include "PrescribedExpansion.h"

#include "MassInviscidFlux.h"
#include "MomentumInviscidFlux.h"
#include "MomentumViscousFlux.h"
#include "EnergyInviscidFlux.h"
#include "EnergyViscousFlux.h"
#include "GravityForce.h"
#include "GravityPower.h"
#include "Velocity.h"
#include "Temperature.h"
#include "DarcyImplicitEuler.h"
#include "DarcyMassFluxPressure.h"
#include "DarcyMassFluxZ.h"
#include "TemperatureImplicitEuler.h"
#include "TemperatureDiffusion.h"
#include "TemperatureConvection.h"
#include "DarcyVelocity.h"
#include "DarcyImplicitBackwardDifference2.h"
#include "TemperatureImplicitBackwardDifference2.h"
#include "CoupledDensityAux.h"
#include "CoupledViscosityAux.h"
#include "AnalyticalADE1D.h"

#include "PressureNeumannBC.h"
#include "GravityNeumannBC.h"
//#include "OutFlowBC.h"


#include "Constant.h"
#include "DarcyWater.h"
#include "ThermalPoroElastic.h"

namespace Falcon
{
  void registerObjects()
  {
    Moose::registerObjects();
    
    KernelFactory::instance()->registerKernel<HeatConduction>("HeatConduction");
    KernelFactory::instance()->registerKernel<HeatConductionImplicitEuler>("HeatConductionImplicitEuler");
    KernelFactory::instance()->registerKernel<SolidMechX>("SolidMechX");
    KernelFactory::instance()->registerKernel<SolidMechY>("SolidMechY");
    KernelFactory::instance()->registerKernel<SolidMechZ>("SolidMechZ");
    KernelFactory::instance()->registerKernel<SolidMechImplicitEuler>("SolidMechImplicitEuler");
    KernelFactory::instance()->registerKernel<SolidMechTempCoupleX>("SolidMechTempCoupleX");
    KernelFactory::instance()->registerKernel<SolidMechTempCoupleY>("SolidMechTempCoupleY");
    KernelFactory::instance()->registerKernel<SolidMechTempCoupleZ>("SolidMechTempCoupleZ");

    KernelFactory::instance()->registerKernel<SolidMechPoroCoupleX>("SolidMechPoroCoupleX");
    KernelFactory::instance()->registerKernel<SolidMechPoroCoupleY>("SolidMechPoroCoupleY");
    KernelFactory::instance()->registerKernel<SolidMechPoroCoupleZ>("SolidMechPoroCoupleZ");

    KernelFactory::instance()->registerKernel<SolidMechSwellingSolid>("SolidMechSwellingSolid");
    KernelFactory::instance()->registerKernel<SolidMechSwellingGas>("SolidMechSwellingGas");
    KernelFactory::instance()->registerKernel<SinHeat>("SinHeat");
    KernelFactory::instance()->registerKernel<PrescribedExpansion>("PrescribedExpansion");

    KernelFactory::instance()->registerKernel<MassInviscidFlux>("MassInviscidFlux");
    KernelFactory::instance()->registerKernel<MomentumInviscidFlux>("MomentumInviscidFlux");
    KernelFactory::instance()->registerKernel<MomentumViscousFlux>("MomentumViscousFlux");
    KernelFactory::instance()->registerKernel<EnergyInviscidFlux>("EnergyInviscidFlux");
    KernelFactory::instance()->registerKernel<EnergyViscousFlux>("EnergyViscousFlux");
    KernelFactory::instance()->registerKernel<GravityForce>("GravityForce");
    KernelFactory::instance()->registerKernel<GravityPower>("GravityPower");
    KernelFactory::instance()->registerKernel<Velocity>("Velocity");
    KernelFactory::instance()->registerKernel<Temperature>("Temperature");
    KernelFactory::instance()->registerKernel<DarcyImplicitEuler>("DarcyImplicitEuler");
    KernelFactory::instance()->registerKernel<DarcyMassFluxPressure>("DarcyMassFluxPressure");
    KernelFactory::instance()->registerKernel<DarcyMassFluxZ>("DarcyMassFluxZ");
    KernelFactory::instance()->registerKernel<TemperatureImplicitEuler>("TemperatureImplicitEuler");
    KernelFactory::instance()->registerKernel<TemperatureDiffusion>("TemperatureDiffusion");
    KernelFactory::instance()->registerKernel<TemperatureConvection>("TemperatureConvection");
    KernelFactory::instance()->registerKernel<DarcyVelocity>("DarcyVelocity");
    KernelFactory::instance()->registerKernel<DarcyImplicitBackwardDifference2>("DarcyImplicitBackwardDifference2");
    KernelFactory::instance()->registerKernel<TemperatureImplicitBackwardDifference2>("TemperatureImplicitBackwardDifference2");

    AuxFactory::instance()->registerAux<CoupledDensityAux>("CoupledDensityAux");
    AuxFactory::instance()->registerAux<CoupledViscosityAux>("CoupledViscosityAux");
    AuxFactory::instance()->registerAux<AnalyticalADE1D>("AnalyticalADE1D");    
        
    BCFactory::instance()->registerBC<PressureNeumannBC>("PressureNeumannBC");
    BCFactory::instance()->registerBC<GravityNeumannBC>("GravityNeumannBC");
    //BCFactory::instance()->registerBC<OutFlowBC>("OutFlowBC");

    MaterialFactory::instance()->registerMaterial<Constant>("Constant");
    MaterialFactory::instance()->registerMaterial<DarcyWater>("DarcyWater");
    MaterialFactory::instance()->registerMaterial<ThermalPoroElastic>("ThermalPoroElastic");
    
  }
}
