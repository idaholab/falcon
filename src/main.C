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
  Parser p;

  srand(libMesh::processor_id());

  Falcon::registerObjects();
  
  std::string input_filename = "";
  if ( Moose::command_line->search("-i") )
    input_filename = Moose::command_line->next(input_filename);
  else
    p.printUsage();

  // Associate Parser Syntax
  Moose::associateSyntax(p);

  p.parse(input_filename);
  p.execute();

  Executioner *e = p.getExecutioner();
  e->execute();
  delete e;

  Moose::perf_log.pop("main()","Falcon");
}

