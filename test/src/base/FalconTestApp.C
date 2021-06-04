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

#include "FalconTestApp.h"
#include "FalconApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters
FalconTestApp::validParams()
{
  InputParameters params = FalconApp::validParams();
  return params;
}

FalconTestApp::FalconTestApp(InputParameters parameters) : MooseApp(parameters)
{
  FalconTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

FalconTestApp::~FalconTestApp() {}

void
FalconTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  FalconApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"FalconTestApp"});
    Registry::registerActionsTo(af, {"FalconTestApp"});
  }
}

void
FalconTestApp::registerApps()
{
  registerApp(FalconApp);
  registerApp(FalconTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
FalconTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  FalconTestApp::registerAll(f, af, s);
}
extern "C" void
FalconTestApp__registerApps()
{
  FalconTestApp::registerApps();
}
