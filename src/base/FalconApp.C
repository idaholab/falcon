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
#include "FalconApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"
#ifdef IAPWS95_ENABLED
#include "IAPWS95App.h"
#endif
#include "ThermalHydraulicsApp.h"
/*******************************************************************************
Input template (do NOT touch)
*******************************************************************************/
InputParameters
FalconApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  // Do not use legacy material output, i.e., output properties on INITIAL as well as TIMESTEP_END
  params.set<bool>("use_legacy_material_output") = false;

  return params;
}

/*******************************************************************************
Routine: FalconApp -- constructor
*******************************************************************************/
FalconApp::FalconApp(InputParameters parameters) : MooseApp(parameters)
{
  FalconApp::registerAll(_factory, _action_factory, _syntax);
}

FalconApp::~FalconApp() {}

void
FalconApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAll(f, af, syntax);
  Registry::registerObjectsTo(f, {"FalconApp"});
  Registry::registerActionsTo(af, {"FalconApp"});
  Registry::registerObjectsTo(f, {"THMTestApp"});

  /* register custom execute flags, action syntax, etc. here */
  ThermalHydraulicsApp::registerAll(f, af, syntax);


#ifdef IAPWS95_ENABLED
  IAPWS95App::registerAll(f, af, syntax);
#endif

// built-in closure types
ThermalHydraulicsApp::registerClosuresOption("falcon", "Closures1PhaseFalcon", THM::FM_SINGLE_PHASE);

}

// External entry point for object registration
extern "C" void
FalconApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  FalconApp::registerAll(f, af, s);
}

/*******************************************************************************
Routine: registerApps (no NOT touch)
*******************************************************************************/
void
FalconApp::registerApps()
{
  registerApp(FalconApp);

#ifdef IAPWS95_ENABLED
  registerApp(IAPWS95App);
#endif
}

extern "C" void
FalconApp__registerApps()
{
  FalconApp::registerApps();
}
