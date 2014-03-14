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
#include "FalconApp.h"
#include "AppFactory.h"
#include "ActionFactory.h"
#include "Syntax.h"

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
#include "MassFluxTimeDerivative_PT_comp.h"
#include "WaterMassFluxPressure_PT.h"
#include "WaterMassFluxElevation_PT.h"
#include "PressureTimeDerivative.h"

//auxkernels
#include "CoupledTemperatureAux.h"         // T as functon of (P,H) -two phase formulation
#include "DarcyFluxAux.h"
#include "VelocityAux.h"
#include "StressStrainDamageComputeAux.h"
#include "FracManMapAux.h"
#include "StochasticFieldAux.h"

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
#include "ChemicalReactions.h"
#include "Geothermal.h"

#include "FracturesPorousMedia.h"
#include "FracturesFluidFlow.h"
#include "FracturesHeatTransport.h"
#include "FracturesSolidMechanics.h"
#include "FracturesChemicalReactions.h"
#include "FracturesGeothermal.h"

#include "StochasticMaterial.h"
#include "StochasticPorousMedia.h"
#include "StochasticFluidFlow.h"
#include "StochasticHeatTransport.h"
#include "StochasticSolidMechanics.h"
#include "StochasticChemicalReactions.h"
#include "StochasticGeothermal.h"

#include "FracManPorousMedia.h"
#include "FracManFluidFlow.h"
#include "FracManHeatTransport.h"
#include "FracManSolidMechanics.h"
#include "FracManChemicalReactions.h"
#include "FracManGeothermal.h"

//userobjects
#include "WaterSteamEOS.h"

//postprocessors
#include "EOSWaterAndSteamPTFuncPPS.h"
#include "EOSPhaseDetermineFuncPPS.h"
#include "EOSViscosityFuncPPS.h"

//actions
#include "GeothermalMaterialAction.h"
#include "StochasticGeothermalMaterialAction.h"
#include "FracManGeothermalMaterialAction.h"
#include "FracturesGeothermalMaterialAction.h"

// Elk stuff
#include "FluidMassEnergyBalanceApp.h"
#include "ChemicalReactionsApp.h"


template<>
InputParameters validParams<FalconApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

FalconApp::FalconApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(libMesh::processor_id());

  Moose::registerObjects(_factory);
  FluidMassEnergyBalanceApp::registerObjects(_factory);
  ChemicalReactionsApp::registerObjects(_factory);
  FalconApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  FluidMassEnergyBalanceApp::associateSyntax(_syntax, _action_factory);
  ChemicalReactionsApp::associateSyntax(_syntax, _action_factory);
  FalconApp::associateSyntax(_syntax, _action_factory);
}


void
FalconApp::registerApps()
{
  registerApp(FalconApp);
}

void
FalconApp::registerObjects(Factory & factory)
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
  registerKernel(MassFluxTimeDerivative_PT_comp);
  registerKernel(WaterMassFluxPressure_PT);
  registerKernel(WaterMassFluxElevation_PT);

  //auxkernels
  registerAux(CoupledTemperatureAux);
  registerAux(DarcyFluxAux);
  registerAux(VelocityAux);
  registerAux(StressStrainDamageComputeAux);
  registerAux(StochasticFieldAux);
  registerAux(FracManMapAux);

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
  registerMaterial(ChemicalReactions);
  registerMaterial(Geothermal);

  registerMaterial(FracturesPorousMedia);
  registerMaterial(FracturesFluidFlow);
  registerMaterial(FracturesHeatTransport);
  registerMaterial(FracturesSolidMechanics);
  registerMaterial(FracturesChemicalReactions);
  registerMaterial(FracturesGeothermal);

  registerMaterial(StochasticMaterial);
  registerMaterial(StochasticPorousMedia);
  registerMaterial(StochasticFluidFlow);
  registerMaterial(StochasticHeatTransport);
  registerMaterial(StochasticSolidMechanics);
  registerMaterial(StochasticChemicalReactions);
  registerMaterial(StochasticGeothermal);

  registerMaterial(FracManPorousMedia);
  registerMaterial(FracManFluidFlow);
  registerMaterial(FracManHeatTransport);
  registerMaterial(FracManSolidMechanics);
  registerMaterial(FracManChemicalReactions);
  registerMaterial(FracManGeothermal);

  //userobjects
  registerUserObject(WaterSteamEOS);

  //postprocessors
  registerPostprocessor(EOSWaterAndSteamPTFuncPPS);
  registerPostprocessor(EOSPhaseDetermineFuncPPS);
  registerPostprocessor(EOSViscosityFuncPPS);
}



void
FalconApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  registerAction(GeothermalMaterialAction, "add_material");
  registerAction(StochasticGeothermalMaterialAction, "add_material");
  registerAction(FracManGeothermalMaterialAction, "add_material");
  registerAction(FracturesGeothermalMaterialAction, "add_material");

  syntax.registerActionSyntax("GeothermalMaterialAction", "Materials/GeothermalMaterial");
  syntax.registerActionSyntax("StochasticGeothermalMaterialAction", "Materials/StochasticGeothermalMaterial");
  syntax.registerActionSyntax("FracManGeothermalMaterialAction", "Materials/FracManGeothermalMaterial");
  syntax.registerActionSyntax("FracturesGeothermalMaterialAction", "Materials/FracturesGeothermalMaterial");

  syntax.registerActionSyntax("RecoverBaseAction", "Output");
  syntax.registerActionSyntax("SetupOutputAction", "Output");
  syntax.registerActionSyntax("SetupOutputNameAction", "Output");
  syntax.registerActionSyntax("SetupOverSamplingAction", "Output/OverSampling");
}
