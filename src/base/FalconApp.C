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

// Kernels
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
#include "TemperatureDiffusion.h"
#include "TemperatureConvection.h"
#include "TemperatureSUPG.h"

#include "MassFluxTimeDerivative_PT.h"
#include "MassFluxTimeDerivative_PT_comp.h"
#include "WaterMassFluxPressure_PT.h"
#include "WaterMassFluxElevation_PT.h"
#include "PressureTimeDerivative.h"

//////////////////////////////////////////////////////////////
//     Miscellaneous                                        //
//////////////////////////////////////////////////////////////
#include "InjectionSourceSink.h"

//////////////////////////////////////////////////////////////
//     Generic diffusion                                    //
//////////////////////////////////////////////////////////////
#include "CoefDiffusion.h"

//////////////////////////////////////////////////////////////
//     Generic convection                                   //
//////////////////////////////////////////////////////////////
#include "Convection.h"
#include "CoupledConvection.h"

//////////////////////////////////////////////////////////////
//     SUPG                                                 //
//////////////////////////////////////////////////////////////
#include "SUPGOneD.h"

// DGKernels
#include "DGConvection.h"
#include "DGCoupledConvection.h"
#include "DGMaterialDiffusion.h"
#include "DGTemperatureConvection.h"

// AuxKernels
#include "CoupledTemperatureAux.h"         // T as functon of (P,H) -two phase formulation
#include "DarcyFluxAux.h"
#include "VelocityAux.h"
#include "FracManMapAux.h"
#include "VariableGradientAux.h"

// BCs
#include "DGConvectionInflowBC.h"
#include "DGConvectionOutflowBC.h"
#include "DGCoupledConvectionInflowBC.h"
#include "DGCoupledConvectionOutflowBC.h"
#include "DGFunctionTemperatureConvectionInflowBC.h"
#include "DGFunctionMaterialDiffusionBC.h"
#include "DGTemperatureConvectionOutflowBC.h"
#include "PressureNeumannBC2.h"
#include "GravityNeumannBC.h"
#include "OutFlowBC.h"
#include "StepDirichletBC.h"
#include "StepPressureBCFunc.h"
#include "PressureBC.h"
#include "PressureOutFlowBC.h"

// ICs

// Materials
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


#include "FracManPorousMedia.h"
#include "FracManFluidFlow.h"
#include "FracManHeatTransport.h"
#include "FracManSolidMechanics.h"
#include "FracManChemicalReactions.h"
#include "FracManGeothermal.h"

// UserObjects
#include "WaterSteamEOS.h"

// PostProcessors

// Actions

////////////////////////////////////////////////////////////////
///      Souce and Sink, volume avagerged                     //
////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
////    Single phase isothermal formulation: pressure        //
///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//       Two phase formulation: pressure & enthalpy         //
//////////////////////////////////////////////////////////////


#include "ChemicalReactionsApp.h"
#include "PhaseFieldApp.h"


template<>
InputParameters validParams<FalconApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

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

extern "C" void FalconApp__registerApps() { FalconApp::registerApps(); }
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

  //SUPG kernels
  registerKernel(SUPGOneD);

  //dgkernels
  registerKernel(DGConvection);
  registerKernel(DGCoupledConvection);
  registerKernel(DGMaterialDiffusion);
  registerKernel(DGTemperatureConvection);

  //auxkernels
  registerAux(CoupledTemperatureAux);
  registerAux(DarcyFluxAux);
  registerAux(VelocityAux);
  registerAux(FracManMapAux);
  registerAux(VariableGradientAux);

  //BCs
  registerNamedBoundaryCondition(PressureNeumannBC2, "PressureNeumannBC");
  registerBoundaryCondition(DGConvectionInflowBC);
  registerBoundaryCondition(DGConvectionOutflowBC);
  registerBoundaryCondition(DGCoupledConvectionInflowBC);
  registerBoundaryCondition(DGCoupledConvectionOutflowBC);
  registerBoundaryCondition(DGFunctionTemperatureConvectionInflowBC);
  registerBoundaryCondition(DGFunctionMaterialDiffusionBC);
  registerBoundaryCondition(DGTemperatureConvectionOutflowBC);
  registerBoundaryCondition(GravityNeumannBC);
  registerBoundaryCondition(OutFlowBC);
  registerBoundaryCondition(StepDirichletBC);
  registerBoundaryCondition(StepPressureBCFunc);
  registerBoundaryCondition(PressureBC);
  registerBoundaryCondition(PressureOutFlowBC);

  // ICs

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


  registerMaterial(FracManPorousMedia);
  registerMaterial(FracManFluidFlow);
  registerMaterial(FracManHeatTransport);
  registerMaterial(FracManSolidMechanics);
  registerMaterial(FracManChemicalReactions);
  registerMaterial(FracManGeothermal);

  //userobjects
  registerUserObject(WaterSteamEOS);

  //postprocessors

  /**
   *fluid mass energy balance objects
   */
  //energy

  //source sink

  //fluid-mass flow-two phase formulation

  //isothermal flow for pressure field
}



void
FalconApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
