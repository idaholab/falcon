/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "Moose.h"
#include "Falcon.h"
#include "Factory.h"

//kernels
///////////////////////////////////////////////////////////////
//      solid mechanics                                      //
///////////////////////////////////////////////////////////////
#include "SolidMechXFalcon.h"
#include "SolidMechYFalcon.h"
#include "SolidMechZFalcon.h"
#include "SolidMechImplicitEuler.h"

#include "SolidMechTempCoupleXFalcon.h"

#include "SolidMechTempCoupleYFalcon.h"
#include "SolidMechTempCoupleZFalcon.h"

#include "SolidMechPoroCoupleX.h"
#include "SolidMechPoroCoupleY.h"
#include "SolidMechPoroCoupleZ.h"
#include "Gravity.h"

///////////////////////////////////////////////////////////////
//      Single phase formulation: pressure & temperature     //
///////////////////////////////////////////////////////////////
#include "TemperatureTimeDerivative.h"
#include "TemperatureTimeDerivativeFluid.h"
#include "TemperatureTimeDerivativeSolid.h"
#include "TemperatureDiffusion.h"
#include "TemperatureConvection.h"

#include "MassFluxTimeDerivative_PT.h"
#include "WaterMassFluxPressure_PT.h"
#include "WaterMassFluxElevation_PT.h"
#include "PressureTimeDerivative.h"

//auxkernels
#include "CoupledTemperatureAux.h"         // T as functon of (P,H) -two phase formulation
#include "VelocityAux.h"
#include "StressStrainDamageComputeAux.h"

//BCs
#include "PressureNeumannBC2.h"
#include "GravityNeumannBC.h"
#include "OutFlowBC.h"
#include "OutFlowBC_PH.h"
#include "StepDirichletBC.h"
#include "StepPressureBCFunc.h"

//ICs
#include "LinearDisEnthalpyIC.h"

//materials
#include "Constant.h"
#include "PorousMedia.h"
#include "FluidFlow.h"
#include "HeatTransport.h"
#include "SolidMechanics.h"
#include "Geothermal.h"

#include "StochasticMaterial.h"
#include "StochasticPorousMedia.h"

#include "StochasticFieldAux.h"
#include "StochasticGeothermal.h"
#include "StochasticFluidFlow.h"
#include "StochasticHeatTransport.h"
#include "StochasticSolidMechanics.h"

//userobjects
#include "WaterSteamEOS.h"

//postprocessors
#include "EOSWaterAndSteamPTFuncPPS.h"
#include "EOSPhaseDetermineFuncPPS.h"
#include "EOSViscosityFuncPPS.h"

namespace Falcon
{

void
registerObjects()
{
//mechanics
  registerNamedKernel(SolidMechXFalcon, "SolidMechXFalcon");
  registerNamedKernel(SolidMechYFalcon, "SolidMechYFalcon");
  registerNamedKernel(SolidMechZFalcon, "SolidMechZFalcon");
  registerKernel(SolidMechImplicitEuler);

  registerKernel(SolidMechTempCoupleXFalcon);
  registerKernel(SolidMechTempCoupleYFalcon);
  registerKernel(SolidMechTempCoupleZFalcon);

  registerKernel(SolidMechPoroCoupleX);
  registerKernel(SolidMechPoroCoupleY);
  registerKernel(SolidMechPoroCoupleZ);
  registerKernel(Gravity);

  //isothermal flow for pressure field
  registerKernel(PressureTimeDerivative);
  //heat transport-PT formulation, single phase only
  registerKernel(TemperatureTimeDerivative);
  registerKernel(TemperatureTimeDerivativeFluid);
  registerKernel(TemperatureTimeDerivativeSolid);
  registerKernel(TemperatureDiffusion);
  registerKernel(TemperatureConvection);
  //fluid-mass flow-single phase formulation
  registerKernel(MassFluxTimeDerivative_PT);
  registerKernel(WaterMassFluxPressure_PT);
  registerKernel(WaterMassFluxElevation_PT);
    
  //auxkernels
  registerAux(CoupledTemperatureAux);
  registerAux(VelocityAux);
  registerAux(StressStrainDamageComputeAux);

  //BCs    
  registerNamedBoundaryCondition(PressureNeumannBC2, "PressureNeumannBC");
  registerBoundaryCondition(GravityNeumannBC);
  registerBoundaryCondition(OutFlowBC);
  registerBoundaryCondition(OutFlowBC_PH);
  registerBoundaryCondition(StepDirichletBC);
  registerBoundaryCondition(StepPressureBCFunc);

  // ICs
  registerInitialCondition(LinearDisEnthalpyIC);
  
  //materials
  registerMaterial(Constant);
  registerMaterial(PorousMedia);
  registerMaterial(FluidFlow);
  registerMaterial(HeatTransport);
  registerMaterial(SolidMechanics);
  registerMaterial(Geothermal);

  registerMaterial(StochasticMaterial);
  registerMaterial(StochasticPorousMedia);
  registerAux(StochasticFieldAux);
  registerMaterial(StochasticGeothermal);
  
  registerMaterial(StochasticFluidFlow);
  registerMaterial(StochasticHeatTransport);
  registerMaterial(StochasticSolidMechanics);
    
  //userobjects
  registerUserObject(WaterSteamEOS);
        
  //postprocessors
  registerPostprocessor(EOSWaterAndSteamPTFuncPPS);
  registerPostprocessor(EOSPhaseDetermineFuncPPS);
  registerPostprocessor(EOSViscosityFuncPPS);
}

}
