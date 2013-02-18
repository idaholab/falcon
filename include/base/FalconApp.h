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

#ifndef FALCONAPP_H
#define FALCONAPP_H

#include "MooseApp.h"

class FalconApp;

template<>
InputParameters validParams<FalconApp>();

class FalconApp : public MooseApp
{
public:
  FalconApp(const std::string & name, InputParameters parameters);
};

#endif //FALCONAPP_H
