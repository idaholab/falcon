/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c)     2012 Battelle Energy Alliance, LLC             */
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
#include "ModulesApp.h"
#include "MooseSyntax.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Actions
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
AuxKernels
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "PTDarcyFluxAux.h"
#include "PTFluidVelocityAux.h"
#include "PTPermeabilityAux.h"
#include "VariableGradientAux.h"


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BCs
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "PTEnergyInFlowBC.h"
#include "PTEnergyOutFlowBC.h"
#include "PTMassFluxBC.h"
#include "PTMassFreeBC.h"


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
#include "PTMassTimeDerivative_FD.h"
#include "HeatSource.h"

// solid mechanics


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Materials
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "PTGeothermal.h"


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
PostProcessors
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "PTEnergySideProductionIntegral.h"
#include "PTMassSideFluxAverage.h"
#include "PTMassSideFluxIntegral.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
UserObjects
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



/*******************************************************************************
Input template (do NOT touch)
*******************************************************************************/
template<>
InputParameters validParams<FalconApp>()
{
  InputParameters params = validParams<MooseApp>();
  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  return params;
}


/*******************************************************************************
Routine: FalconApp -- constructor
*******************************************************************************/
FalconApp::FalconApp(InputParameters parameters) :
    MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  FalconApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
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
  registerAux(PTPermeabilityAux);
  registerAux(VariableGradientAux);


  /* BCs */

  registerBoundaryCondition(PTEnergyInFlowBC);
  registerBoundaryCondition(PTEnergyOutFlowBC);
  registerBoundaryCondition(PTMassFluxBC);
  registerBoundaryCondition(PTMassFreeBC);

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
  registerKernel(PTMassTimeDerivative_FD);
  registerKernel(HeatSource);


  // solid mechanics


  /* Materials */

  registerMaterial(PTGeothermal);


  /* PostProcessors */
  registerPostprocessor(PTEnergySideProductionIntegral);
  registerPostprocessor(PTMassSideFluxAverage);
  registerPostprocessor(PTMassSideFluxIntegral);

  /* UserObjects */
}


/*******************************************************************************
Routine: registerApps
*******************************************************************************/
void
FalconApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{}
