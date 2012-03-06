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

#include "Falcon.h"

//Moose Includes
#include "MooseInit.h"
#include "Executioner.h"

// Parser
#include "Parser.h"
#include "MooseSyntax.h"

// C++ include files
#include <iostream>

// libMesh includes
#include "perf_log.h"

PerfLog Moose::perf_log("Falcon");

 // Begin the main program.
int main (int argc, char** argv)
{
  Moose::perf_log.push("main()","Falcon");

  MooseInit init (argc, argv);

  srand(libMesh::processor_id());

  Falcon::registerObjects();
  // Associate Parser Syntax
  Moose::associateSyntax();
  Parser p(Moose::syntax);

  // Parse commandline and return inputfile filename if appropriate
  std::string input_filename = p.parseCommandLine();

  p.parse(input_filename);
  p.execute();

  Moose::executioner->execute();

  Moose::perf_log.pop("main()","Falcon");
}

