#include "Falcon.h"

//Moose Includes
#include "Parser.h"
#include "Moose.h"
#include "Executioner.h"

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

  Parser p;

  std::string input_filename = "";
  if ( Moose::command_line->search("-i") )
    input_filename = Moose::command_line->next(input_filename);
  else
    mooseError("Must specify an input file using -i");      

  p.parse(input_filename);
  p.execute();

  if(!Moose::executioner)
    mooseError("Executioner not supplied!");

  Moose::executioner->setup();
  Moose::executioner->execute();

  Moose::perf_log.pop("main()","Falcon");
}
