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

/*******************************************************************************
Input template (do NOT touch)
*******************************************************************************/
InputParameters FalconApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  params.set<bool>("use_legacy_dirichlet_bc") = false;
  return params;
}


registerKnownLabel("FalconApp");

/*******************************************************************************
Routine: FalconApp -- constructor
*******************************************************************************/
FalconApp::FalconApp(InputParameters parameters) :
    MooseApp(parameters)
{
  FalconApp::registerAll(_factory, _action_factory, _syntax);
}

// External entry point for object registration
extern "C" void
FalconApp__registerAll(Factory & factory, ActionFactory & action_factory, Syntax & syntax)
{
  FalconApp::registerAll(factory, action_factory, syntax);
}

void
FalconApp::registerAll(Factory & factory, ActionFactory & action_factory, Syntax & syntax)
{
  Registry::registerObjectsTo(factory, {"FalconApp"});
  Registry::registerActionsTo(action_factory, {"FalconApp"});
  ModulesApp::registerAll(factory, action_factory, syntax);
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
