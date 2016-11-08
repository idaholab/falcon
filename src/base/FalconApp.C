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
#include "MooseSyntax.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Actions
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
AuxKernels
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "FracManMapAux.h"
#include "PTDarcyFluxAux.h"
#include "PTFluidVelocityAux.h"
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
#include "PTEnergyPointSourceSchedule.h"
#include "PTMassPointSource.h"
#include "PTMassPointSourceSchedule.h"


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
#include "PTFracGeothermal.h"
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

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MOOSE physics modules
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//#include "PhaseFieldApp.h"


/*******************************************************************************
Input template (do NOT touch)
*******************************************************************************/
template<>
InputParameters validParams<FalconApp>()
{
  InputParameters params = validParams<MooseApp>();
  params.set<bool>("use_legacy_output_syntax") = false;
  return params;
}


/*******************************************************************************
Routine: FalconApp -- constructor
*******************************************************************************/
FalconApp::FalconApp(InputParameters parameters) :
    MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  //PhaseFieldApp::registerObjects(_factory);
  FalconApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  //PhaseFieldApp::associateSyntax(_syntax, _action_factory);
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

  registerAux(FracManMapAux);
  registerAux(PTDarcyFluxAux);
  registerAux(PTFluidVelocityAux);
  registerAux(VariableGradientAux);


  /* BCs */

  registerBoundaryCondition(PTEnergyInFlowBC);
  registerBoundaryCondition(PTEnergyOutFlowBC);
  registerBoundaryCondition(PTMassFluxBC);
  registerBoundaryCondition(PTMassFreeBC);

  /* DGKernels */


  /* DiracKernels */
  registerDiracKernel(PTEnergyPointSource);
  registerDiracKernel(PTEnergyPointSourceSchedule);
  registerDiracKernel(PTMassPointSource);
  registerDiracKernel(PTMassPointSourceSchedule);


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

  registerMaterial(PTFracGeothermal);
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
