#include "Moose.h"
#include "Factory.h"

//mechanics
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

//heat transport
#include "TemperatureTimeDerivative.h"
#include "TemperatureTimeDerivativeFluid.h"
#include "TemperatureTimeDerivativeSolid.h"
#include "TemperatureDiffusion.h"
#include "TemperatureConvection.h"
//#include "Temperature.h"

//fluid-mass flow
#include "MassFluxTimeDerivative.h"
#include "WaterMassFluxPressure.h"
#include "SteamMassFluxPressure.h"
#include "WaterMassFluxElevation.h"
//#include "CoupledDarcyImplicitEuler.h"

//energy
#include "EnthalpyImplicitEuler.h"
#include "EnthalpyDiffusion.h"                  \
  //#include "EnthalpyConvectionWater.h"
//#include "EnthalpyConvectionSteam.h"

//auxkernels
#include "CoupledDensityAux.h"
#include "CoupledDdensityDTAux.h"
#include "CoupledDdensityDPAux.h"
#include "CoupledViscosityAux.h"
#include "AnalyticalADE1D.h"
//#include "TemperatureAux.h"
//#include "WaterSatAux.h"/
//#include "CoupledRhoAux.h"
#include "VelocityAux.h"
#include "CoupledPorosityMaterialAux.h"
#include "CoupledPorosityNodalAux.h"
//#include "CoupledWaterDensityAux.h"

//BCs
#include "PressureNeumannBC2.h"
#include "GravityNeumannBC.h"
#include "OutFlowBC.h"
#include "StepDirichletBC.h"


//materials
#include "Constant.h"
#include "PorousMedia.h"
#include "FluidFlow.h"
#include "HeatTransport.h"
#include "SolidMechanics.h"
#include "Geothermal.h"


namespace Falcon
{
  void registerObjects()
  {
    Moose::registerObjects();
    
//mechanics
    registerNamedKernel(SolidMechXFalcon, "SolidMechX");
    registerNamedKernel(SolidMechYFalcon, "SolidMechY");
    registerNamedKernel(SolidMechZFalcon, "SolidMechZ");
    registerKernel(SolidMechImplicitEuler);
    
    registerKernel(StressCompute);
    registerKernel(StrainCompute);

    registerNamedKernel(SolidMechTempCoupleXFalcon, "SolidMechTempCoupleX");
    registerNamedKernel(SolidMechTempCoupleYFalcon, "SolidMechTempCoupleY");
    registerNamedKernel(SolidMechTempCoupleZFalcon, "SolidMechTempCoupleZ");

    registerKernel(SolidMechPoroCoupleX);
    registerKernel(SolidMechPoroCoupleY);
    registerKernel(SolidMechPoroCoupleZ);

    registerKernel(SolidMechSwellingSolid);
    registerKernel(SolidMechSwellingGas);
    registerKernel(SinHeat);

//heat transport
    registerKernel(TemperatureTimeDerivative);
    registerKernel(TemperatureTimeDerivativeFluid);
    registerKernel(TemperatureTimeDerivativeSolid);
    registerKernel(TemperatureDiffusion);
    registerKernel(TemperatureConvection);
//    registerKernel(Temperature);

//fluid-mass flow    
    registerKernel(MassFluxTimeDerivative);
    registerKernel(WaterMassFluxPressure);
    registerKernel(SteamMassFluxPressure);
    registerKernel(WaterMassFluxElevation);
    
//    registerKernel(CoupledDarcyImplicitEuler);
    
//energy
    registerKernel(EnthalpyImplicitEuler);
    registerKernel(EnthalpyDiffusion);
//    registerKernel(EnthalpyConvectionWater);
    //   registerKernel(EnthalpyConvectionSteam);

//auxkernels
    registerAux(CoupledDdensityDTAux);
    registerAux(CoupledDdensityDPAux);    
    registerAux(CoupledDensityAux);
    registerAux(CoupledViscosityAux);
    registerAux(AnalyticalADE1D);
    //  registerAux(TemperatureAux);
    // registerAux(WaterSatAux);
    // registerAux(CoupledRhoAux);
    registerAux(VelocityAux);
    registerAux(CoupledPorosityMaterialAux);
    registerAux(CoupledPorosityNodalAux);
    //registerAux(CoupledWaterDensityAux);
    
//BCs    
    registerNamedBoundaryCondition(PressureNeumannBC2, "PressureNeumannBC");
    registerBoundaryCondition(GravityNeumannBC);
    registerBoundaryCondition(OutFlowBC);
    registerBoundaryCondition(StepDirichletBC);

//materials    
    registerMaterial(Constant);
    registerMaterial(PorousMedia);
    registerMaterial(FluidFlow);
    registerMaterial(HeatTransport);
    registerMaterial(SolidMechanics);
    registerMaterial(Geothermal);

    
  }
}
