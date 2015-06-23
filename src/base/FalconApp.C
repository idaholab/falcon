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
Actions
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
AuxKernels
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "PTDarcyFluxAux.h"
#include "PTFluidVelocityAux.h"
#include "VariableGradientAux.h"


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BCs
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "PTEnergyInFlowBC.h"
#include "PTEnergyOutFlowBC.h"


#include "PressureNeumannBC2.h"
#include "GravityNeumannBC.h"
#include "StepDirichletBC.h"
#include "StepPressureBCFunc.h"
#include "PressureBC.h"
#include "PressureOutFlowBC.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DGKernels
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DiracKernels
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "PTEnergyPointSource.h"
#include "PTMassPointSource.h"


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ICs
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Kernels
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

// pressure & temperature based single-phase flow and heat
#include "PTEnergyResidual.h"
#include "PTEnergyTimeDerivative.h"
#include "PTMassResidual.h"
#include "PTMassTimeDerivative.h"

// solid mechanics
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


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Materials
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "PTGeothermal.h"
#include "PorousMedia.h"
#include "SolidMechanics.h"


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
PostProcessors
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UserObjects
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MOOSE physics modules
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
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
  PhaseFieldApp::registerObjects(_factory);
  FalconApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
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
  /* AuxKernels */

  registerAux(PTDarcyFluxAux);
  registerAux(PTFluidVelocityAux);
  registerAux(VariableGradientAux);


  /* BCs */

  registerBoundaryCondition(PTEnergyInFlowBC);
  registerBoundaryCondition(PTEnergyOutFlowBC);

  registerNamedBoundaryCondition(PressureNeumannBC2, "PressureNeumannBC");
  registerBoundaryCondition(GravityNeumannBC);
  registerBoundaryCondition(StepDirichletBC);
  registerBoundaryCondition(StepPressureBCFunc);
  registerBoundaryCondition(PressureBC);
  registerBoundaryCondition(PressureOutFlowBC);

  /* DGKernels */


  /* DiracKernels */
  registerDiracKernel(PTEnergyPointSource);
  registerDiracKernel(PTMassPointSource);


  /* ICs */


  /* Kernel */

  // pressure & temperature based single-phase flow and heat
  registerKernel(PTEnergyResidual);
  registerKernel(PTEnergyTimeDerivative);
  registerKernel(PTMassResidual);
  registerKernel(PTMassTimeDerivative);


  // solid mechanics
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


  /* Materials */

  registerMaterial(PTGeothermal);
  registerMaterial(PorousMedia);
  registerMaterial(SolidMechanics);


  /* PostProcessors */


  /* UserObjects */

}


/*******************************************************************************
Routine: registerApps
*******************************************************************************/
void
FalconApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{}
