#include "Moose.h"
#include "MooseFactory.h"

#include "HeatConduction.h"
#include "HeatConductionImplicitEuler.h"
#include "SolidMechXFalcon.h"
#include "SolidMechYFalcon.h"
#include "SolidMechZFalcon.h"
#include "SolidMechImplicitEuler.h"

#include "StressCompute.h"
#include "StrainCompute.h"

#include "SolidMechTempCoupleXFalcon.h"
#include "SolidMechTempCoupleYFalcon.h"
#include "SolidMechTempCoupleZFalcon.h"

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
#include "CoupledDarcyImplicitEuler.h"
#include "DarcyMassFluxPressure.h"
#include "DarcyMassFluxPressureSteam.h"
#include "DarcyMassFluxZ.h"
#include "DarcyMassFluxZSteam.h"
#include "TemperatureImplicitEuler.h"
#include "TemperatureDiffusion.h"
#include "TemperatureConvection.h"
#include "EnthalpyImplicitEuler.h"
#include "EnthalpyDiffusion.h"
#include "EnthalpyConvection.h"
#include "EnthalpyConvectionZWater.h"
#include "EnthalpyConvectionZSteam.h"
#include "HuyakornMassImplicitEuler.h"
#include "HuyakornMassConvection.h"
#include "HuyakornEnthalpyImplicitEuler.h"
#include "HuyakornEnthalpyDiffusion.h"
#include "HuyakornEnthalpyConvection.h"
#include "DarcyVelocity.h"
#include "DarcyImplicitBackwardDifference2.h"
#include "TemperatureImplicitBackwardDifference2.h"

#include "CoupledDensityAux.h"
#include "CoupledViscosityAux.h"
#include "AnalyticalADE1D.h"
#include "TemperatureAux.h"
#include "WaterSatAux.h"
#include "CoupledRhoAux.h"
#include "VelocityAux.h"

#include "PressureNeumannBC2.h"
#include "GravityNeumannBC.h"
#include "OutFlowBC.h"
#include "HuyakornBC.h"
#include "StepDirichletBC.h"

#include "Constant.h"
#include "DarcyWater.h"
#include "ThermalPoroElastic.h"
#include "PorousMedia.h"
#include "FluidFlow.h"
#include "FluidFlow2Phase.h"
#include "HuyakornFluidFlow2Phase.h"
#include "HeatTransport.h"
#include "SolidMechanics.h"
#include "Geothermal.h"
#include "Geothermal2Phase.h"

namespace Falcon
{
  void registerObjects()
  {
    Moose::registerObjects();
    
    registerKernel(HeatConduction);
    registerKernel(HeatConductionImplicitEuler);
    registerNamedKernel(SolidMechXFalcon, "SolidMechX");
    registerNamedKernel(SolidMechYFalcon, "SolidMechY");
    registerNamedKernel(SolidMechZFalcon, "SolidMechZ");
    registerKernel(StressCompute);
    registerKernel(StrainCompute);
    registerKernel(SolidMechImplicitEuler);
    registerNamedKernel(SolidMechTempCoupleXFalcon, "SolidMechTempCoupleX");
    registerNamedKernel(SolidMechTempCoupleYFalcon, "SolidMechTempCoupleY");
    registerNamedKernel(SolidMechTempCoupleZFalcon, "SolidMechTempCoupleZ");

    registerKernel(SolidMechPoroCoupleX);
    registerKernel(SolidMechPoroCoupleY);
    registerKernel(SolidMechPoroCoupleZ);

    registerKernel(SolidMechSwellingSolid);
    registerKernel(SolidMechSwellingGas);
    registerKernel(SinHeat);
    registerKernel(PrescribedExpansion);

    registerKernel(MassInviscidFlux);
    registerKernel(MomentumInviscidFlux);
    registerKernel(MomentumViscousFlux);
    registerKernel(EnergyInviscidFlux);
    registerKernel(EnergyViscousFlux);
    registerKernel(GravityForce);
    registerKernel(GravityPower);
    registerKernel(Velocity);
    registerKernel(Temperature);
    registerKernel(DarcyImplicitEuler);
    registerKernel(CoupledDarcyImplicitEuler);
    registerKernel(HuyakornMassImplicitEuler);
    registerKernel(DarcyMassFluxPressure);
    registerKernel(HuyakornMassConvection);
    registerKernel(DarcyMassFluxPressureSteam);
    registerKernel(DarcyMassFluxZ);
    registerKernel(DarcyMassFluxZSteam);
    registerKernel(TemperatureImplicitEuler);
    registerKernel(TemperatureDiffusion);
    registerKernel(TemperatureConvection);
    registerKernel(EnthalpyImplicitEuler);
    registerKernel(HuyakornEnthalpyImplicitEuler);
    registerKernel(EnthalpyDiffusion);
    registerKernel(HuyakornEnthalpyDiffusion);
    registerKernel(EnthalpyConvection);
    registerKernel(HuyakornEnthalpyConvection);
    registerKernel(EnthalpyConvectionZWater);
    registerKernel(EnthalpyConvectionZSteam);
    registerKernel(DarcyVelocity);
    registerKernel(DarcyImplicitBackwardDifference2);
    registerKernel(TemperatureImplicitBackwardDifference2);

    registerAux(CoupledDensityAux);
    registerAux(CoupledViscosityAux);
    registerAux(AnalyticalADE1D);
    registerAux(TemperatureAux);
    registerAux(WaterSatAux);
    registerAux(CoupledRhoAux);
    registerAux(VelocityAux);
        
    registerNamedBC(PressureNeumannBC2, "PressureNeumannBC");
    registerBC(GravityNeumannBC);
    registerBC(OutFlowBC);
    registerBC(HuyakornBC);
    registerBC(StepDirichletBC);

    registerMaterial(Constant);
    registerMaterial(DarcyWater);
    registerMaterial(ThermalPoroElastic);
    registerMaterial(PorousMedia);
    registerMaterial(FluidFlow);
    registerMaterial(FluidFlow2Phase);
    registerMaterial(HuyakornFluidFlow2Phase);
    registerMaterial(HeatTransport);
    registerMaterial(SolidMechanics);
    registerMaterial(Geothermal);
    registerMaterial(Geothermal2Phase);
    
  }
}
