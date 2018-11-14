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

#ifndef FALCONAPP_H
#define FALCONAPP_H

#include "MooseApp.h"

class FalconApp;

template<>
InputParameters validParams<FalconApp>();

class FalconApp : public MooseApp
{
public:
  FalconApp(InputParameters parameters);

  static void registerAll(Factory & factory, ActionFactory & action_factory, Syntax & syntax);
  static void registerApps();
};

#endif //FALCONAPP_H
