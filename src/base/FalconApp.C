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

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Baseline dependencies (do NOT touch)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "Moose.h"
#include "FalconApp.h"
#include "AppFactory.h"
#include "ActionFactory.h"
#include "Syntax.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Kernels
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

// ===============
// solid mechanics
// ===============
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

// ================================================
// single phase formulation: pressure & temperature
// ================================================
#include "TemperatureTimeDerivative.h"
#include "TemperatureDiffusion.h"
#include "TemperatureConvection.h"
#include "TemperatureSUPG.h"

#include "MassFluxTimeDerivative_PT.h"
#include "MassFluxTimeDerivative_PT_comp.h"
#include "WaterMassFluxPressure_PT.h"
#include "WaterMassFluxElevation_PT.h"
#include "PressureTimeDerivative.h"

#include "InjectionSourceSink.h"

#include "CoefDiffusion.h"

#include "Convection.h"
#include "CoupledConvection.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DGKernels
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
AuxKernels
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "DarcyFluxAux.h"
#include "VelocityAux.h"
#include "FracManMapAux.h"
#include "VariableGradientAux.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BCs
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "PressureNeumannBC2.h"
#include "GravityNeumannBC.h"
#include "OutFlowBC.h"
#include "StepDirichletBC.h"
#include "StepPressureBCFunc.h"
#include "PressureBC.h"
#include "PressureOutFlowBC.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ICs
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Materials
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "Constant.h"
#include "PorousMedia.h"
#include "FluidFlow.h"
#include "HeatTransport.h"
#include "SolidMechanics.h"
#include "ChemicalReactions.h"
#include "Geothermal.h"
#include "GeoProcPT.h"

#include "FracturesPorousMedia.h"
#include "FracturesFluidFlow.h"
#include "FracturesHeatTransport.h"
#include "FracturesSolidMechanics.h"
#include "FracturesChemicalReactions.h"
#include "FracturesGeothermal.h"


#include "FracManPorousMedia.h"
#include "FracManFluidFlow.h"
#include "FracManHeatTransport.h"
#include "FracManSolidMechanics.h"
#include "FracManChemicalReactions.h"
#include "FracManGeothermal.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UserObjects
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "WaterSteamEOS.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
PostProcessors
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Actions
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MOOSE physics modules
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "ChemicalReactionsApp.h"
#include "PhaseFieldApp.h"


/*******************************************************************************
Input template (do NOT touch)
*******************************************************************************/
template<>
InputParameters validParams<FalconApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}


/*******************************************************************************
Routine: FalconApp -- constructor
*******************************************************************************/
FalconApp::FalconApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(processor_id());

  Moose::registerObjects(_factory);
  ChemicalReactionsApp::registerObjects(_factory);
  PhaseFieldApp::registerObjects(_factory);
  FalconApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ChemicalReactionsApp::associateSyntax(_syntax, _action_factory);
  PhaseFieldApp::associateSyntax(_syntax, _action_factory);
  FalconApp::associateSyntax(_syntax, _action_factory);
}


/*******************************************************************************
Routine: registerApps (no NOT touch)
*******************************************************************************/
extern "C" void FalconApp__registerApps() { FalconApp::registerApps(); }
void
FalconApp::registerApps()
{
  registerApp(FalconApp);
}


/*******************************************************************************
Routine: registerObjects
*******************************************************************************/
void
FalconApp::registerObjects(Factory & factory)
{
  // ===============
  // solid mechanics
  // ===============
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
  registerKernel(TemperatureDiffusion);
  registerKernel(TemperatureConvection);
  registerKernel(TemperatureSUPG);
  //fluid-mass flow-single phase formulation
  registerKernel(MassFluxTimeDerivative_PT);
  registerKernel(MassFluxTimeDerivative_PT_comp);
  registerKernel(WaterMassFluxPressure_PT);
  registerKernel(WaterMassFluxElevation_PT);

  //miscellaneous kernels
  registerKernel(InjectionSourceSink);

  //generic diffusion kernels
  registerKernel(CoefDiffusion);

  //generic convection kernels
  registerKernel(Convection);
  registerKernel(CoupledConvection);

  // =========
  // DGKernels
  // =========

  // ==========
  // AuxKernels
  // ==========
  registerAux(DarcyFluxAux);
  registerAux(VelocityAux);
  registerAux(FracManMapAux);
  registerAux(VariableGradientAux);

  // ===
  // BCs
  // ===
  registerNamedBoundaryCondition(PressureNeumannBC2, "PressureNeumannBC");
  registerBoundaryCondition(GravityNeumannBC);
  registerBoundaryCondition(OutFlowBC);
  registerBoundaryCondition(StepDirichletBC);
  registerBoundaryCondition(StepPressureBCFunc);
  registerBoundaryCondition(PressureBC);
  registerBoundaryCondition(PressureOutFlowBC);

  // ===
  // ICs
  // ===

  // =========
  // Materials
  // =========
  registerMaterial(Constant);
  registerMaterial(PorousMedia);
  registerMaterial(FluidFlow);
  registerMaterial(HeatTransport);
  registerMaterial(SolidMechanics);
  registerMaterial(ChemicalReactions);
  registerMaterial(Geothermal);
  registerMaterial(GeoProcPT);

  registerMaterial(FracturesPorousMedia);
  registerMaterial(FracturesFluidFlow);
  registerMaterial(FracturesHeatTransport);
  registerMaterial(FracturesSolidMechanics);
  registerMaterial(FracturesChemicalReactions);
  registerMaterial(FracturesGeothermal);


  registerMaterial(FracManPorousMedia);
  registerMaterial(FracManFluidFlow);
  registerMaterial(FracManHeatTransport);
  registerMaterial(FracManSolidMechanics);
  registerMaterial(FracManChemicalReactions);
  registerMaterial(FracManGeothermal);

  // ===========
  // UserObjects
  // ===========
  registerUserObject(WaterSteamEOS);

  // ==============
  // PostProcessors
  // ==============

}


/*******************************************************************************
Routine: registerApps
*******************************************************************************/
void
FalconApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{}
