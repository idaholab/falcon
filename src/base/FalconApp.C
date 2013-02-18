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
#include "FalconApp.h"

#include "FluidMassEnergyBalanceModule.h"

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
  Elk::FluidMassEnergyBalance::registerObjects(_factory);
  Falcon::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
}
