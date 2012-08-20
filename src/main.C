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

#include "FalconApp.h"
#include "MooseInit.h"
#include "Moose.h"
// libMesh includes
#include "perf_log.h"

PerfLog Moose::perf_log("Falcon");

 // Begin the main program.
int main (int argc, char** argv)
{
  Moose::perf_log.push("main()","Falcon");

  MooseInit init (argc, argv);
  FalconApp app(argc, argv);
  app.run();

  return 0;
}

