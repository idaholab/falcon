#include "Falcon.h"
#include "PetscSupport.h"

//Moose Includes
#include "Moose.h"
#include "KernelFactory.h"
#include "BCFactory.h"
#include "MaterialFactory.h"
#include "PhysicsBasedPreconditioner.h"
#include "AuxFactory.h"
#include "ComputeResidual.h"
#include "ComputeJacobian.h"

// C++ include files that we need
#include <iostream>
#include <fstream>
#include <iomanip>

// libMesh includes
#include "libmesh.h"
#include "mesh.h"
#include "mesh_refinement.h"
#include "mesh_generation.h"
#include "boundary_info.h"
#include "gmv_io.h"
#include "exodusII_io.h"
#include "tecplot_io.h"
#include "equation_systems.h"
#include "string_to_enum.h"
#include "getpot.h"
#include "error_vector.h"
#include "kelly_error_estimator.h"
#include "mesh_refinement.h"
#include "nonlinear_solver.h"
#include "nonlinear_implicit_system.h"
#include "linear_implicit_system.h"
#include "transient_system.h"
#include "perf_log.h"
#include "string_to_enum.h"
#include "parallel.h"
#include "linear_partitioner.h"
#include "numeric_vector.h"

PerfLog Moose::perf_log("Falcon");

Number init_value (const Point& p,
                   const Parameters& parameters,
                   const std::string& sys_name,
                   const std::string& var_name)
{
  /*
  if(var_name == "p")
    return 101325.0 / (287.04 * 600.0);

  if(var_name == "pe")
    return 101325.0 / (1.405 - 1.0);

  if(var_name == "c")
    return -1+(2.0*rand()/(RAND_MAX + 1.0));
  */

  if(var_name == "cv" || var_name == "c")
    return 1.13e-4*rand()/(RAND_MAX + 1.0);
/*
  if(var_name == "temp")
  {
    if (p(1) <= -0.499)
      return 1.0;
    else
      0.0;
  }
*/

  if(parameters.have_parameter<Real>("initial_"+var_name))
    return parameters.get<Real>("initial_"+var_name);
  
  return 0;
}

Gradient init_gradient (const Point& p,
                        const Parameters& parameters,
                        const std::string& sys_name,
                        const std::string& var_name)
{
  /*
  if(var_name == "p")
    return 101325.0 / (287.04 * 600.0);

  if(var_name == "pe")
    return 101325.0 / (1.405 - 1.0);
  */
  if(parameters.have_parameter<Real>("initial_"+var_name))
    return parameters.get<Real>("initial_"+var_name);
  
  return 0;
}

void init_cond(EquationSystems& es, const std::string& system_name)
{
  ExplicitSystem & system = es.get_system<ExplicitSystem>(system_name);
  
  system.project_solution(init_value, init_gradient, es.parameters);
}

/**
 * Small helper function for printing parsing errors.
 */
void print_missing_param(std::string missing_param, std::string object_name)
{
  std::cerr<<std::endl<<"A _"<<missing_param<<"_ must be specified for "<<object_name<<std::endl<<std::endl;
  libmesh_error();
}

/**
 * Outputs the system.
 */
void output_system(EquationSystems & equation_systems, std::string file_base, unsigned int t_step, Real time, bool exodus_output, bool gmv_output, bool tecplot_output, bool print_out_info)
{
  OStringStream stream_file_base;
  
  stream_file_base << file_base << "_";
  OSSRealzeroright(stream_file_base,3,0,t_step);

  std::string file_name = stream_file_base.str();

  if(print_out_info)
     std::cout << "   --> Output in file ";
  
  if(exodus_output) 
  {     
    static ExodusII_IO ex_out(equation_systems.get_mesh());
    // The +1 is because Exodus starts timesteps at 1 and we start at 0
    ex_out.write_timestep(file_base + ".e", equation_systems, t_step+1, time);
    
    if(print_out_info)
    {       
      std::cout << file_base+".e";
      OStringStream out;
      out <<  "(";
      OSSInt(out,2,t_step+1);
      out <<  ") ";
      std::cout << out.str();
    }
    
  }  
  if(gmv_output) 
  {     
    GMVIO(*Moose::mesh).write_equation_systems(file_name + ".gmv", equation_systems);
    if(print_out_info)
    {       
      if(exodus_output)
         std::cout << " and ";    
      std::cout << file_name+".gmv";
    }    
  }  
  if(tecplot_output) 
  {     
    TecplotIO(*Moose::mesh).write_equation_systems(file_name + ".plt", equation_systems);
    if(print_out_info)
    {       
      if(exodus_output || gmv_output)
         std::cout << " and ";    
      std::cout << file_name+".plt";
    }    
  }

  if(print_out_info) std::cout << std::endl; 
}

void dump() 
{
  // Dump Kernel Parameters
  std::cout << "\nKERNEL\n";
  for (KernelNamesIterator i = KernelFactory::instance()->registeredKernelsBegin();
       i != KernelFactory::instance()->registeredKernelsEnd(); ++i)
  {
    Parameters tmpParams = KernelFactory::instance()->getValidParams(*i);

    std::cout<<'<' << *i << ">\n";
    std::cout << std::setiosflags(std::ios::left);
    for (Parameters::iterator j = tmpParams.begin(); j != tmpParams.end(); ++j)
    {
      std::cout << "\t" << std::setw(35) << j->first;//<< "\"" << "True" << "\"\n";
      j->second->print(std::cout);
      std::cout<< "\n";
      }
    
    std::cout<<"</" << *i << ">\n";
    
  }
 
  // Dump BoundaryCondition Parameters
  std::cout << "\nBOUNDARY_CONDITION\n";
  for (BCNamesIterator i = BCFactory::instance()->registeredBCsBegin();
       i != BCFactory::instance()->registeredBCsEnd(); ++i)
  {
    Parameters tmpParams = BCFactory::instance()->getValidParams(*i);
    std::cout<<'<' << *i << ">\n"; 
   
    for (Parameters::iterator j = tmpParams.begin(); j != tmpParams.end(); ++j)
    {
      std::cout << "\t" << std::setw(35) << j->first;// << "\"" << "True" << "\"\n";
      j->second->print(std::cout);
      std::cout<< "\n";
    }
    
    std::cout<<"</" << *i << ">\n";
    
  }

  // Dump Material Parameters
  std::cout << "\nMATERIAL\n";
  for (MaterialNamesIterator i = MaterialFactory::instance()->registeredMaterialsBegin();
       i != MaterialFactory::instance()->registeredMaterialsEnd(); ++i)
  {
    Parameters tmpParams = MaterialFactory::instance()->getValidParams(*i);

    std::cout<<'<' << *i << ">\n"; 
    for (Parameters::iterator j = tmpParams.begin(); j != tmpParams.end(); ++j)
    {
      std::cout << "\t" << std::setw(35) << j->first;// << "\"" << "True" << "\"\n";
      j->second->print(std::cout);
      std::cout<< "\n";
    }
    
    std::cout<<"</" << *i << ">\n";
  }
  std::resetiosflags(std::ios::left);
}

// Begin the main program.
int main (int argc, char** argv)
{
  Moose::perf_log.push("main()","Falcon");

  // Initialize libMesh and any dependent libaries, like in example 2.
  libMesh::init (argc, argv);

  //Seed the random number generator
  srand(libMesh::processor_id());

  Falcon::registerObjects();
  
  // Braces are used to force object scope, like in example 2
   {
    // Create a GetPot object to parse the command line
    GetPot command_line (argc, argv);

    if (command_line.search("--dump")) 
    {
      dump();

      // Exit the program without solving any problem
      exit(0);
    }
    
    std::string input_filename = "";
    if ( command_line.search("-i") ) 
    {
      
      input_filename = command_line.next(input_filename);

      // Check if file exists
      std::ifstream in(input_filename.c_str(), std::ifstream::in);
      if (in.fail()) 
      {
        
        std::cout<<"Unable to open file \"" << input_filename
                 << "\".  Check to make sure that it exists and that you have read permission." << std::endl;
        libmesh_error();
      }
    }
    else
    {
      std::cout<<"Must specify an input file using -i"<<std::endl;
      libmesh_error();
    }

    GetPot input_file(input_filename);

#ifdef LIBMESH_HAVE_PETSC
    PetscSupport::petscParseOptions(input_file);
#endif //HAVE_PETSC    
    
    if(!input_file("Execution/perf_log",false))
      Moose::perf_log.disable_logging();

    unsigned int mesh_dim = input_file("Mesh/dim",3);
    unsigned int init_unif_refine = input_file("Mesh/uniform_refine",0);
    std::string mesh_file = input_file("Mesh/file","");
    bool second_order = input_file("Mesh/second_order",false);

    const unsigned int dim = mesh_dim;

    Mesh *mesh = new Mesh(dim);

    // MUST set the global mesh!
    Moose::mesh = mesh;
    
    ExodusII_IO * exreader = NULL;

    bool generated = input_file("Mesh/generated",false);

    if(!generated)
    {
      /* Detect if any solution variables need to be read and determine if we need to use
       * the Exodus reader or if the mesh reader will suffice */
      int num_vars = input_file.vector_variable_size("Variables/names");
      bool restart_detected = false;
      for(int i=0;i<num_vars && !restart_detected;i++)
      {
        std::string var_name = input_file("Variables/names", "", i);
        if(input_file.vector_variable_size(("Variables/" + var_name + "/initial_from_file").c_str())) 
          restart_detected = true;
      }
      int num_aux_vars = input_file.vector_variable_size("AuxVariables/names");
      for(int i=0;i<num_aux_vars && !restart_detected;i++)
      {
        std::string var_name = input_file("AuxVariables/names", "", i);
        if(input_file.vector_variable_size(("AuxVariables/" + var_name + "/initial_from_file").c_str())) 
          restart_detected = true;
      }

      if (restart_detected) 
      {
        exreader = new ExodusII_IO(*mesh);
        exreader->read(mesh_file);
      }
      else
        /* We will use the mesh object to read the file to cut down on
         * I/O conntention.  We still need to use the Exodus reader though
         *for copy_nodal_solutions
         */
        mesh->read(mesh_file);  
    }
    else
    {
      unsigned int nx = input_file("Mesh/Generation/nx",1);
      unsigned int ny = input_file("Mesh/Generation/ny",1);
      unsigned int nz = input_file("Mesh/Generation/nz",1);

      double xmin = input_file("Mesh/Generation/xmin",0.0);
      double ymin = input_file("Mesh/Generation/ymin",0.0);
      double zmin = input_file("Mesh/Generation/zmin",0.0);

      double xmax = input_file("Mesh/Generation/xmax",1.0);
      double ymax = input_file("Mesh/Generation/ymax",1.0);
      double zmax = input_file("Mesh/Generation/zmax",1.0);

      ElemType elem_type = Utility::string_to_enum<ElemType>(input_file("Mesh/Generation/elem_type","QUAD4"));
//      MeshTools::Generation::build_cube(*mesh, nx, ny, nz, xmin, xmax, xmin, xmax, xmin, xmax, elem_type, false, 1, 1);
//      MeshTools::Generation::build_square(*mesh, nx, ny, xmin, xmax, ymin, ymax, elem_type, false);
    }

    if(second_order)
      mesh->all_second_order(true);

    std::string partitioner = input_file("Mesh/partitioner", "");
    
    if (partitioner == "linear")
      mesh->partitioner() = AutoPtr<Partitioner>(new LinearPartitioner);
    mesh->prepare_for_use(false);
    
    // If using ParallelMesh this will delete non-local elements from the current processor
    mesh->delete_remote_elements();

    MeshRefinement mesh_refinement(*mesh);

    mesh_refinement.uniformly_refine(init_unif_refine);

    //Sets node's boundary ids from their side's boundary ids
    mesh->boundary_info->build_node_list_from_side_list();

//    mesh->boundary_info->add_node(mesh->node_ptr(4294),4);
//    mesh->boundary_info->print_info();

    mesh->print_info();
    
    EquationSystems equation_systems (*mesh);

    // MUST set the global equation_systems!
    Moose::equation_system = &equation_systems;
    
    TransientNonlinearImplicitSystem& system =
      equation_systems.add_system<TransientNonlinearImplicitSystem> ("NonlinearSystem");

    equation_systems.parameters.set<Real> ("linear solver tolerance")
      = input_file("Execution/l_tol",1.e-5);

    double l_abs_step_tol = input_file("Execution/l_abs_step_tol",-1.0);
    
    equation_systems.parameters.set<unsigned int> ("linear solver maximum iterations")
      = input_file("Execution/l_max_its",10000);    
    
    equation_systems.parameters.set<unsigned int> ("nonlinear solver maximum iterations")
      = input_file("Execution/nl_max_its",50);

    equation_systems.parameters.set<unsigned int> ("nonlinear solver maximum function evaluations")
      = input_file("Execution/nl_max_funcs",10000);

    equation_systems.parameters.set<Real> ("nonlinear solver absolute residual tolerance")
      = input_file("Execution/nl_abs_tol",1.e-50);

    equation_systems.parameters.set<Real> ("nonlinear solver relative residual tolerance")
      = input_file("Execution/nl_rel_tol",1.e-8);

    equation_systems.parameters.set<Real> ("nonlinear solver absolute step tolerance")
      = input_file("Execution/nl_abs_step_tol",1.e-50);

    equation_systems.parameters.set<Real> ("nonlinear solver relative step tolerance")
      = input_file("Execution/nl_rel_step_tol",1.e-50);

    Moose::no_fe_reinit = input_file("Execution/no_fe_reinit",false);
    
    bool physics_based_preconditioning = input_file("Execution/pbp",false);
    
    //Map of variables to be read from a file to the timestep to be read.
    std::map<std::string, unsigned int> var_to_timestep;
    std::map<std::string, unsigned int> aux_var_to_timestep;
      
    //Parse the variables out
    int num_vars = input_file.vector_variable_size("Variables/names");
    for(int i=0;i<num_vars;i++)
    {
      std::string prefix = "Variables/";

      std::string var_name = input_file("Variables/names", "", i);

      std::string order_name = prefix + var_name + "/order";
      std::string order = input_file(order_name.c_str(), "");

      if(order == "")
	print_missing_param("order",var_name);

      std::string family_name = prefix + var_name + "/family";
      std::string family = input_file(family_name.c_str(), "");

      if(family == "")
	print_missing_param("family",var_name);
      
      std::string initial_name = prefix + var_name + "/initial_condition";
      Real initial = input_file(initial_name.c_str(), 0.0);

      if(initial > 1e-12 || initial < -1e-12)
        equation_systems.parameters.set<Real>("initial_"+var_name) = initial;
      
      system.add_variable(var_name,
			  Utility::string_to_enum<Order>   (order),
			  Utility::string_to_enum<FEFamily>(family));

      if(physics_based_preconditioning)
      {
        //Create the preconditioning system
        LinearImplicitSystem & precond_system = equation_systems.add_system<LinearImplicitSystem>(var_name+"_system");
        precond_system.assemble_before_solve = false;
        precond_system.add_variable(var_name+"_prec",
                                    Utility::string_to_enum<Order>   (order),
                                    Utility::string_to_enum<FEFamily>(family));
      }      

      if(input_file.vector_variable_size((prefix + var_name + "/initial_from_file").c_str()))
      {
        std::string read_name = input_file((prefix + var_name + "/initial_from_file").c_str(), "", 0);

        //Default to reading timestep 2 (which will be the final solution from a steady state calc
        int read_timestep = input_file((prefix + var_name + "/initial_from_file").c_str(), 2, 1);
        
        var_to_timestep[read_name] = read_timestep;
      }
    }

    system.nonlinear_solver->residual = Moose::compute_residual;

    if(!physics_based_preconditioning)
      system.nonlinear_solver->jacobian = Moose::compute_jacobian;

    system.attach_init_function(init_cond);

    
    

    if(physics_based_preconditioning)
    {
      PhysicsBasedPreconditioner *precond = new PhysicsBasedPreconditioner();
      
      //Parse out the PBP options
      input_file.set_prefix("Execution/PBP/");

      unsigned int num_solve_order = input_file.vector_variable_size("solve_order");
      
      if(num_solve_order && num_solve_order < system.n_vars())
      {
        std::cout<<num_solve_order<<std::endl;
        
        std::cerr<<std::endl<<"If you specify a solve_order it must contain all of the variables"<<std::endl;
        libmesh_error();
      }

      std::vector<unsigned int> solve_order(num_solve_order);
      
      for(int i=0;i<num_solve_order;i++)
        solve_order[i] = system.variable_number(input_file("solve_order","",i));

      unsigned int num_pre = input_file.vector_variable_size("preconditioner");
      
      if(num_pre && num_pre != system.n_vars())
      {
        std::cerr<<std::endl<<"If you specify the preconditioner you must specify it for all of the variables"<<std::endl;
        libmesh_error();
      }

      std::vector<PreconditionerType> pre(num_pre);

      for(int i=0;i<num_pre;i++)
        pre[i] = Utility::string_to_enum<PreconditionerType>(input_file("preconditioner","",i));

      unsigned int num_off_diag_row = input_file.vector_variable_size("off_diag_row");
      unsigned int num_off_diag_column = input_file.vector_variable_size("off_diag_column");

      if(num_off_diag_row != num_off_diag_column)
      {
        std::cerr<<"off_diag_row and off_diag_column must be the same size!"<<std::endl;
        libmesh_error();
      }

      std::vector<std::vector<unsigned int> > off_diag(system.n_vars());

      for(int i=0;i<num_off_diag_row;i++)
      {
        unsigned int row = system.variable_number(input_file("off_diag_row","",i));
        unsigned int column = system.variable_number(input_file("off_diag_column","",i));

        //The +1 is because the preconditioning system is always 1 more than the variable number
        LinearImplicitSystem & u_system = equation_systems.get_system<LinearImplicitSystem>(row+1);

        //Add the matrix to hold the off diagonal piece
        u_system.add_matrix(input_file("off_diag_column","",i));
        
        off_diag[row].push_back(column);
      }  
      
      input_file.set_prefix("");
      
      precond->setEq(equation_systems);
      precond->setComputeJacobianBlock(Moose::compute_jacobian_block);
      precond->setSolveOrder(solve_order);
      precond->setPreconditionerType(pre);
      precond->setOffDiagBlocks(off_diag);

      system.nonlinear_solver->attach_preconditioner(precond);
    }

    TransientExplicitSystem& aux_system =
      equation_systems.add_system<TransientExplicitSystem> ("AuxiliarySystem");

    aux_system.attach_init_function(init_cond);

    //Parse the aux variables out
    int num_aux_vars = input_file.vector_variable_size("AuxVariables/names");

    for(int i=0;i<num_aux_vars;i++)
    {
      std::string prefix = "AuxVariables/";

      std::string var_name = input_file("AuxVariables/names", "", i);

      std::string order_name = prefix + var_name + "/order";
      std::string order = input_file(order_name.c_str(), "");

      if(order == "")
	print_missing_param("order",var_name);

      std::string family_name = prefix + var_name + "/family";
      std::string family = input_file(family_name.c_str(), "");

      if(family == "")
	print_missing_param("family",var_name);

      std::string initial_name = prefix + var_name + "/initial_condition";
      Real initial = input_file(initial_name.c_str(), 0.0);

      if(initial > 1e-12 || initial < -1e-12)
        equation_systems.parameters.set<Real>("initial_"+var_name) = initial;
      
      aux_system.add_variable(var_name,
                              Utility::string_to_enum<Order>   (order),
                              Utility::string_to_enum<FEFamily>(family));

      if(input_file.vector_variable_size((prefix + var_name + "/initial_from_file").c_str()))
      {
        std::string read_name = input_file((prefix + var_name + "/initial_from_file").c_str(), "", 0);
        
        //Default to reading timestep 2 (which will be the final solution from a steady state calc
        int read_timestep = input_file((prefix + var_name + "/initial_from_file").c_str(), 2, 1);
        
        aux_var_to_timestep[read_name] = read_timestep;
      }
    }

    equation_systems.init();
    equation_systems.print_info();

    if (var_to_timestep.size() || aux_var_to_timestep.size()) 
    {
      if (exreader == NULL)
        libmesh_error();
      
      std::map<std::string, unsigned int>::iterator var_it = var_to_timestep.begin();
      const std::map<std::string, unsigned int>::iterator var_end = var_to_timestep.end();

      for(;var_it != var_end; ++var_it)
        exreader->copy_nodal_solution(system, var_it->first, var_it->second);

      std::map<std::string, unsigned int>::iterator aux_var_it = aux_var_to_timestep.begin();
      const std::map<std::string, unsigned int>::iterator aux_var_end = aux_var_to_timestep.end();

      for(;aux_var_it != aux_var_end; ++aux_var_it)
        exreader->copy_nodal_solution(aux_system, aux_var_it->first, aux_var_it->second);
    }
    

#ifdef LIBMESH_HAVE_PETSC
    PetscSupport::petscSetDefaults(Moose::equation_system,system, Moose::compute_jacobian_block, Moose::compute_residual);
#endif //LIBMESH_HAVE_PETSC
    
    // Grab the Execution parameters
    std::string execution_type = "Steady";
    execution_type = input_file("Execution/type",execution_type);

    if(execution_type != "Transient" && execution_type != "Steady")
    {
      std::cerr<<std::endl<<"Must specify either Transient or Steady for Execution/type"<<std::endl;
      libmesh_error();
    }

    if(execution_type == "Transient")
    {
      double time                    = input_file("Execution/Transient/start_time",0.0);
      double end_time                = input_file("Execution/Transient/end_time",1.e+30);
      double dt                      = input_file("Execution/Transient/dt",-1.0);
      double dtmin                   = input_file("Execution/Transient/dtmin",0.0);
      double dtmax                   = input_file("Execution/Transient/dtmax",1.0e30);
      int num_steps                  = input_file("Execution/Transient/num_steps",-1);
      int n_startup_steps            = input_file("Execution/Transient/n_startup_steps",0);
      bool adaptive_time_stepping    = input_file("Execution/Transient/adaptive_time_stepping",false);
      bool sol_time_adaptive_time_stepping    = input_file("Execution/Transient/sol_time_adaptive_time_stepping",false);
      bool exponential_time_stepping = input_file("Execution/Transient/exponential_time_stepping",false);
      bool trans_ss_check            = input_file("Execution/Transient/trans_ss_check",false);
      double ss_check_tol            = input_file("Execution/Transient/ss_check_tol",1.e-08);
      double ss_tmin                 = input_file("Execution/Transient/ss_tmin",0.);
      
      double reject_step_error       = input_file("Execution/Transient/reject_step_error",-1.);
      if(reject_step_error > 0)
      {
        system.add_vector("time_error");
        system.add_vector("old_solution");
      }

      // Transient simulation run time checks
      if(dt < 0)
	print_missing_param("dt","Transient");
      if(num_steps < 0)
	print_missing_param("num_steps","Transient");

      // Load transient simulation control parameters 
      equation_systems.parameters.set<Real> ("time")                      = time;
      equation_systems.parameters.set<Real> ("end_time")                  = end_time;
      equation_systems.parameters.set<Real> ("dt")                        = dt;
      equation_systems.parameters.set<Real> ("dtmin")                     = dtmin;
      equation_systems.parameters.set<Real> ("dtmax")                     = dtmax;
      equation_systems.parameters.set<int>  ("num_steps")                 = num_steps;
      equation_systems.parameters.set<int>  ("n_startup_steps")           = n_startup_steps;
      equation_systems.parameters.set<bool> ("adaptive_time_stepping")    = adaptive_time_stepping;
      equation_systems.parameters.set<bool> ("sol_time_adaptive_time_stepping")    = sol_time_adaptive_time_stepping;
      equation_systems.parameters.set<bool> ("exponential_time_stepping") = exponential_time_stepping;
      equation_systems.parameters.set<bool> ("trans_ss_check")            = trans_ss_check;
      equation_systems.parameters.set<Real> ("ss_check_tol")              = ss_check_tol;
      equation_systems.parameters.set<Real> ("ss_tmin")                   = ss_tmin;
      equation_systems.parameters.set<Real> ("reject_step_error")         = reject_step_error;      
    }

    //Adaptivity objects
    unsigned int max_r_steps = 0;
    ErrorVector error;
    KellyErrorEstimator error_estimator;

    bool adaptivity = input_file("Execution/adaptivity",false);
    
    //Read Adaptivity Parameters
    if(adaptivity)
    {      
      input_file.set_prefix("Execution/Adaptivity/");

      max_r_steps = input_file("steps",0);

      mesh_refinement.refine_fraction()  = input_file("refine_fraction",0.0);
      mesh_refinement.coarsen_fraction() = input_file("coarsen_fraction",0.0);
      mesh_refinement.max_h_level()      = input_file("max_h_level",0);

      int num_weight_names = input_file.vector_variable_size("weight_names");
      int num_weight_values = input_file.vector_variable_size("weight_values");

      if(num_weight_names)
      {
        if(num_weight_names != num_weight_values)
        {
          std::cerr<<std::endl<<"Number of weight_names must be equal to number of weight_values in Execution/Adaptivity"<<std::endl;
          libmesh_error();
        }

        // If weights have been specified then set the default weight to zero
        std::vector<float> weights(system.n_vars(),0);

        for(int i=0;i<num_weight_names;i++)
        {
          std::string name = input_file("weight_names", "", i);
          double value = input_file("weight_values", 0.0, i);

          weights[system.variable_number(name)] = value;
        }
        
//        error_estimator.component_scale = weights;
      }

      input_file.set_prefix("");
    }

    //Initialize common data structures for Kernels
    Kernel::init(&equation_systems);
    BoundaryCondition::init();
    AuxKernel::init();

    //Parse the kernels out
    num_vars = input_file.vector_variable_size("Kernels/names");
    for(int i=0;i<num_vars;i++)
    {
      std::string prefix = "Kernels/";

      std::string kernel_name = input_file("Kernels/names", "", i);

      std::string type_name = prefix + kernel_name + "/type";
      std::string type = input_file(type_name.c_str(), "");

      if(type == "")
	print_missing_param("type",kernel_name);

      std::string var_name = prefix + kernel_name + "/variable";
      std::string var = input_file(var_name.c_str(), "");

      if(var == "")
	print_missing_param("variable",kernel_name);

      std::string block_name = prefix + kernel_name + "/block";
      int block = input_file(block_name.c_str(), -1);

      Parameters kernel_params = KernelFactory::instance()->getValidParams(type);

      Parameters::iterator it = kernel_params.begin();
      Parameters::iterator end = kernel_params.end();

      for(;it != end; ++it)
      {
	Parameters::Parameter<Real> * param = dynamic_cast<Parameters::Parameter<Real>*>(it->second);
        param->set()=input_file((prefix+kernel_name+"/"+it->first).c_str(), param->get());
      }

      int num_coupled_to = input_file.vector_variable_size((prefix + kernel_name + "/coupled_to").c_str());
      int num_coupled_as = input_file.vector_variable_size((prefix + kernel_name + "/coupled_as").c_str());

      if(num_coupled_to != num_coupled_as)
      {
        std::cerr<<std::endl<<"_coupled_to_ must be the same size as _coupled_as_ for "<<kernel_name<<std::endl;
        libmesh_error();
      }

      std::vector<std::string> coupled_to;
      std::vector<std::string> coupled_as;

      for(int j=0;j<num_coupled_to;j++)
      {
        coupled_to.push_back(input_file((prefix + kernel_name + "/coupled_to").c_str(), "", j));
        coupled_as.push_back(input_file((prefix + kernel_name + "/coupled_as").c_str(), "", j));
      }                       

      if(block < 0)
        KernelFactory::instance()->add(type, kernel_name, kernel_params, var, coupled_to, coupled_as);
      else
        KernelFactory::instance()->add(type, kernel_name, kernel_params, var, coupled_to, coupled_as, block);
    }

    //Parse the AuxKernels out
    num_vars = input_file.vector_variable_size("AuxKernels/names");
    for(int i=0;i<num_vars;i++)
    {
      std::string prefix = "AuxKernels/";

      std::string kernel_name = input_file("AuxKernels/names", "", i);

      std::string type_name = prefix + kernel_name + "/type";
      std::string type = input_file(type_name.c_str(), "");

      if(type == "")
	print_missing_param("type",kernel_name);

      std::string var_name = prefix + kernel_name + "/variable";
      std::string var = input_file(var_name.c_str(), "");

      if(var == "")
	print_missing_param("variable",kernel_name);

      Parameters kernel_params = AuxFactory::instance()->getValidParams(type);

      Parameters::iterator it = kernel_params.begin();
      Parameters::iterator end = kernel_params.end();

      for(;it != end; ++it)
      {
	Parameters::Parameter<Real> * param = dynamic_cast<Parameters::Parameter<Real>*>(it->second);
        param->set()=input_file((prefix+kernel_name+"/"+it->first).c_str(), param->get());
      }

      int num_coupled_to = input_file.vector_variable_size((prefix + kernel_name + "/coupled_to").c_str());
      int num_coupled_as = input_file.vector_variable_size((prefix + kernel_name + "/coupled_as").c_str());

      if(num_coupled_to != num_coupled_as)
      {
        std::cerr<<std::endl<<"_coupled_to_ must be the same size as _coupled_as_ for "<<kernel_name<<std::endl;
        libmesh_error();
      }

      std::vector<std::string> coupled_to;
      std::vector<std::string> coupled_as;

      for(int j=0;j<num_coupled_to;j++)
      {
        coupled_to.push_back(input_file((prefix + kernel_name + "/coupled_to").c_str(), "", j));
        coupled_as.push_back(input_file((prefix + kernel_name + "/coupled_as").c_str(), "", j));
      }                       

      AuxFactory::instance()->add(type, kernel_name, kernel_params, var, coupled_to, coupled_as);
    }

    //Parse the BCs out
    num_vars = input_file.vector_variable_size("BCs/names");
    for(int i=0;i<num_vars;i++)
    {
      std::string prefix = "BCs/";

      std::string bc_name = input_file("BCs/names", "", i);

      input_file.set_prefix((prefix + bc_name + "/").c_str());

      std::string type = input_file("type", "");

      if(type == "")
	print_missing_param("type",bc_name);

      std::string var = input_file("variable", "");

      if(var == "")
	print_missing_param("variable",bc_name);

      Parameters bc_params = BCFactory::instance()->getValidParams(type);

      Parameters::iterator it = bc_params.begin();
      Parameters::iterator end = bc_params.end();

      for(;it != end; ++it)
      {
	Parameters::Parameter<Real> * param = dynamic_cast<Parameters::Parameter<Real>*>(it->second);
	param->set()=input_file(it->first.c_str(), param->get());
      }

      int num_coupled_to = input_file.vector_variable_size("coupled_to");
      int num_coupled_as = input_file.vector_variable_size("coupled_as");

      if(num_coupled_to != num_coupled_as)
      {
        std::cerr<<std::endl<<"_coupled_to_ must be the same size as _coupled_as_ for "<<bc_name<<std::endl;
        libmesh_error();
      }

      std::vector<std::string> coupled_to;
      std::vector<std::string> coupled_as;

      for(int j=0;j<num_coupled_to;j++)
      {
        coupled_to.push_back(input_file("coupled_to", "", j));
        coupled_as.push_back(input_file("coupled_as", "", j));
      }

      int num_boundary = input_file.vector_variable_size("boundary");

      if(num_boundary <= 0)
	print_missing_param("boundary",bc_name);

      //Add one BC for each boundary
      for(int j=0;j<num_boundary;j++)
      {
        
        int boundary = input_file("boundary", -1, j);

        if(boundary > 0)
          BCFactory::instance()->add(type, bc_name, bc_params, var, boundary, coupled_to, coupled_as);
        else
        {
          std::cout<<"Invalid boundary number: "<<boundary<<std::endl;
          libmesh_error();
        }
      }  

      input_file.set_prefix("");
    }

    //Parse the aux BCs out
    num_vars = input_file.vector_variable_size("AuxBCs/names");
    for(int i=0;i<num_vars;i++)
    {
      std::string prefix = "AuxBCs/";

      std::string bc_name = input_file("AuxBCs/names", "", i);

      input_file.set_prefix((prefix + bc_name + "/").c_str());

      std::string type = input_file("type", "");

      if(type == "")
	print_missing_param("type",bc_name);

      std::string var = input_file("variable", "");

      if(var == "")
	print_missing_param("variable",bc_name);

      Parameters bc_params = AuxFactory::instance()->getValidParams(type);

      Parameters::iterator it = bc_params.begin();
      Parameters::iterator end = bc_params.end();

      for(;it != end; ++it)
      {
	Parameters::Parameter<Real> * param = dynamic_cast<Parameters::Parameter<Real>*>(it->second);
	param->set()=input_file(it->first.c_str(), param->get());
      }

      int num_coupled_to = input_file.vector_variable_size("coupled_to");
      int num_coupled_as = input_file.vector_variable_size("coupled_as");

      if(num_coupled_to != num_coupled_as)
      {
        std::cerr<<std::endl<<"_coupled_to_ must be the same size as _coupled_as_ for "<<bc_name<<std::endl;
        libmesh_error();
      }

      std::vector<std::string> coupled_to;
      std::vector<std::string> coupled_as;

      for(int j=0;j<num_coupled_to;j++)
      {
        coupled_to.push_back(input_file("coupled_to", "", j));
        coupled_as.push_back(input_file("coupled_as", "", j));
      }

      int num_boundary = input_file.vector_variable_size("boundary");

      if(num_boundary <= 0)
	print_missing_param("boundary",bc_name);

      //Add one BC for each boundary
      for(int j=0;j<num_boundary;j++)
      {
        
        int boundary = input_file("boundary", -1, j);

        if(boundary > 0)
          AuxFactory::instance()->addBC(type, bc_name, bc_params, var, boundary, coupled_to, coupled_as);
        else
        {
          std::cout<<"Invalid boundary number: "<<boundary<<std::endl;
          libmesh_error();
        }
      }  

      input_file.set_prefix("");
    }

    //Parse the materials out
    num_vars = input_file.vector_variable_size("Materials/names");
    for(int i=0;i<num_vars;i++)
    {
      std::string prefix = "Materials/";

      std::string material_name = input_file("Materials/names", "", i);

      std::string type_name = prefix + material_name + "/type";
      std::string type = input_file(type_name.c_str(), "");

      if(type == "")
	print_missing_param("type",material_name);

      std::string block_name = prefix + material_name + "/block";
      int block = input_file(block_name.c_str(), -1);

      if(block < 0)
	print_missing_param("block",material_name);

      Parameters material_params = MaterialFactory::instance()->getValidParams(type);

      Parameters::iterator it = material_params.begin();
      Parameters::iterator end = material_params.end();

      for(;it != end; ++it)
      {
	Parameters::Parameter<Real> * param = dynamic_cast<Parameters::Parameter<Real>*>(it->second);
        param->set()=input_file((prefix+material_name+"/"+it->first).c_str(), param->get());
      }

      int num_coupled_to = input_file.vector_variable_size((prefix + material_name + "/coupled_to").c_str());
      int num_coupled_as = input_file.vector_variable_size((prefix + material_name + "/coupled_as").c_str());

      if(num_coupled_to != num_coupled_as)
      {
        std::cerr<<std::endl<<"_coupled_to_ must be the same size as _coupled_as_ for "<<material_name<<std::endl;
        libmesh_error();
      }

      std::vector<std::string> coupled_to;
      std::vector<std::string> coupled_as;

      for(int j=0;j<num_coupled_to;j++)
      {
        coupled_to.push_back(input_file((prefix + material_name + "/coupled_to").c_str(), "", j));
        coupled_as.push_back(input_file((prefix + material_name + "/coupled_as").c_str(), "", j));
      }                       

      MaterialFactory::instance()->add(type, material_name, material_params, block, coupled_to, coupled_as);
    }
    
    //Get Output Params
    input_file.set_prefix("Output/");

    std::string file_base = "";
    file_base = input_file("file_base",file_base);
    if(file_base == "")
      print_missing_param("file_base","Output");

    int interval = input_file("interval",1);

    bool exodus_output = input_file("exodus",false);
    bool gmv_output = input_file("gmv",false);
    bool tecplot_output = input_file("tecplot",false);

    if(!exodus_output && !gmv_output && !tecplot_output)
    {
      std::cerr<<std::endl<<"No ouput format specified, needs at least one of: exodus, gmv, tecplot"<<std::endl;
      libmesh_error();
    }

    bool print_out_info = input_file("print_out_info",false);
    
    if(input_file("output_initial",false))
       output_system(equation_systems, file_base, 0, 0.0, exodus_output, gmv_output, tecplot_output,print_out_info);

    bool converged = true;
    bool step_rejected = false;
    
    if(execution_type == "Transient")
    {
      // Load time steppping prameters for Transient scope
      double time                    = equation_systems.parameters.get<Real>("time");
      double end_time                = equation_systems.parameters.get<Real>("end_time");
      double dt                      = equation_systems.parameters.get<Real>("dt");
      double dtmin                   = equation_systems.parameters.get<Real>("dtmin");
      double dtmax                   = equation_systems.parameters.get<Real>("dtmax");
      int num_steps                  = equation_systems.parameters.get<int>("num_steps");
      int n_startup_steps            = equation_systems.parameters.get<int>("n_startup_steps");
      bool adaptive_time_stepping    = equation_systems.parameters.get<bool>("adaptive_time_stepping");
      bool sol_time_adaptive_time_stepping    = equation_systems.parameters.get<bool>("sol_time_adaptive_time_stepping");
      bool exponential_time_stepping = equation_systems.parameters.get<bool>("exponential_time_stepping");
      bool trans_ss_check            = equation_systems.parameters.get<bool>("trans_ss_check");
      double ss_check_tol            = equation_systems.parameters.get<Real>("ss_check_tol");
      double ss_tmin                 = equation_systems.parameters.get<Real>("ss_tmin");      
      double reject_step_error       = equation_systems.parameters.get<Real>("reject_step_error");
      
      bool keep_going = true;
      int & t_step = equation_systems.parameters.set<int> ("t_step") = 0;
      Real dt_cur = dt;

      // Used in adaptive time stepping
      Real initial_residual_norm = 0;
      Real last_residual_norm = 0;
      Real largest_residual_norm = 0;
      Real largest_dt = 0;
      
      Real sol_time_vs_dt = 1e18;
      Real old_sol_time_vs_dt = 1e19;
      Real older_sol_time_vs_dt = 1e20;
      Real optimum_sol_time_vs_dt = 1e20;

      //When positive, dt will be increasing
      Real adaptive_direction = 1;

      std::ofstream adaptive_log;
      
      if(libMesh::processor_id() == 0)
        adaptive_log.open("adaptive_log");
      
      // Used to check for steady-state on a transient problem
      Real old_time_solution_norm = 0;
      
      // Start time loop...
      while(keep_going) 
      {
        std::cout<<"In transient loop"<<std::endl;
        
        system.update();

        if(converged)
        {
          // Update backward time solution vectors
          *system.older_local_solution = *system.old_local_solution;
          *system.old_local_solution   = *system.current_local_solution;

          *aux_system.older_local_solution = *aux_system.old_local_solution;
          *aux_system.old_local_solution   = *aux_system.current_local_solution;

          // Update backward material data structures
          MaterialFactory::instance()->updateMaterialDataState();
        }
        
        // Increment number of time steps
        if(converged)
          t_step++;
        
        // If start up steps are needed
        if(t_step == 1 && n_startup_steps > 1)
          dt_cur = dt/(double)(n_startup_steps);
        else
        {
          // No adaptive time stepping 
          if((adaptive_time_stepping==false && sol_time_adaptive_time_stepping==false) || t_step == 1)
          {
            // Set time step:
            dt_cur = dt;

            // Time step definition might need to be in it's own kernal
            if(exponential_time_stepping)
              dt_cur = std::exp((double)(t_step-1) / 20.0);

            // Don't let the time step size exceed maximum time step size
            if(dt_cur > dtmax)
              dt_cur = dtmax;

            // Don't allow time step size to be smaller than minimum time step size
            if(dt_cur < dtmin)
              dt_cur = dtmin;
          
            // Don't let time go beyond simulation end time
            if(time + dt_cur > end_time)
              dt_cur = end_time - time;
          
            // Increment time
            time += dt_cur;
          
            equation_systems.parameters.set<Real> ("time")  = time;
            equation_systems.parameters.set<Real> ("dt")    = dt_cur;
            equation_systems.parameters.set<int> ("t_step") = t_step;
        
            Kernel::reinitDT();

            std::cout<<"Computing Residual 1"<<std::endl;

            Moose::compute_residual(*system.current_local_solution,*system.rhs);
            system.rhs->close();

            std::cout<<"Finished Computing Residual 1"<<std::endl;

            initial_residual_norm = system.rhs->l2_norm();
            last_residual_norm = initial_residual_norm;

            std::cout<<"Initial Residual Norm: "<<initial_residual_norm<<std::endl;
          }
          else // Adaptive time stepping is used
          {
            if(converged)
            {
              if(adaptive_time_stepping) //old algorithm
              {
                if(last_residual_norm > largest_residual_norm)
                {
                  std::cout<<"Largest Norm Reset To:"<<last_residual_norm<<std::endl;
                  largest_residual_norm = last_residual_norm;
                  largest_dt = dt_cur;
                }

                Real temp_dt = dt_cur * 1.2;

                Real temp_time = time + temp_dt;
          
                equation_systems.parameters.set<Real> ("time")  = temp_time;
                equation_systems.parameters.set<Real> ("dt")    = temp_dt;
                equation_systems.parameters.set<int> ("t_step") = t_step;
        
                Kernel::reinitDT();

                Moose::compute_residual(*system.current_local_solution,*system.rhs);

                system.rhs->close();
            
                Real temp_residual_norm = system.rhs->l2_norm();
 
                std::cout<<"Ratio: "<<(largest_residual_norm / temp_residual_norm)<<std::endl;

                //Modify the timestep based on the ratio of residual norms
                temp_dt = temp_dt * (largest_residual_norm / temp_residual_norm);

                //Keep growth to 10%
                if(temp_dt > dt_cur * 1.1)
                  temp_dt = dt_cur * 1.1;

                dt_cur = temp_dt;

                if(exponential_time_stepping)
                {
                  Real exponential = time / 20.0;
                  std::cout<<"Exponential: "<<exponential<<std::endl;
                  if(dt_cur > exponential)
                  {
                    std::cout<<"Limiting dt to exponential " << std::endl;
                    dt_cur = exponential;
                  }
                }
              }
              else // Solution time adaptivity
              {
                if(libMesh::processor_id() == 0)
                {
                  adaptive_log<<"Old Ratio: "<<old_sol_time_vs_dt<<std::endl;
                  adaptive_log<<"New Ratio: "<<sol_time_vs_dt<<std::endl;
                }
                
                //Ratio grew so switch direction
                if(sol_time_vs_dt > older_sol_time_vs_dt && old_sol_time_vs_dt > older_sol_time_vs_dt)
                {
                  if(libMesh::processor_id() == 0)
                    adaptive_log<<"Switching Direction"<<std::endl;
                  
                  adaptive_direction *= -1;
                }

                dt_cur += dt_cur * 0.1 * adaptive_direction;                
              }
            }
            else //Solve failed... cut timestep
            {
              if(last_residual_norm < largest_residual_norm)
                largest_residual_norm = last_residual_norm;
              
              if(libMesh::processor_id() == 0)
                adaptive_log<<"Solve Failed... cutting timestep"<<std::endl;
              time -= dt_cur;
              dt_cur*=0.5;

              //If the step was rejected based on an error criteria then current_local_solution
              //has already been modified
              if(!step_rejected)
                *system.current_local_solution = *system.old_local_solution;
            }
          
            // Don't let the time step size exceed maximum time step size
            if(dt_cur > dtmax)
              dt_cur = dtmax;

            // Don't allow time step size to be smaller than minimum time step size
            if(dt_cur < dtmin)
              dt_cur = dtmin;
          
            // Don't let time go beyond simulation end time
            if(time + dt_cur > end_time)
              dt_cur = end_time - time;
          
            // Increment time
            time += dt_cur;

            equation_systems.parameters.set<Real> ("time")  = time;
            equation_systems.parameters.set<Real> ("dt")    = dt_cur;

            Kernel::reinitDT();
          }
        }
        Moose::compute_residual(*system.current_local_solution,*system.rhs);
        system.rhs->close();

        last_residual_norm = system.rhs->l2_norm();
        
        std::cout<<"DT: "<<dt_cur<<std::endl;

        if(libMesh::processor_id() == 0)
          adaptive_log<<"Cur DT: "<<dt_cur<<std::endl;
          
        std::cout << " Solving time step ";
        {
          OStringStream out;
  
          OSSInt(out,2,t_step);
          out << ", time=";
          OSSRealzeroleft(out,9,6,time);
          out <<  "...";
          std::cout << out.str() << std::endl;
        }

#ifdef LIBMESH_HAVE_PETSC
        PetscSupport::petscSetDefaults(Moose::equation_system, system, Moose::compute_jacobian_block, Moose::compute_residual);
#endif //LIBMESH_HAVE_PETSC

        timeval solve_start, solve_end;
        gettimeofday (&solve_start, NULL);  

        Moose::perf_log.push("solve()","Solve");
        // System Solve
        system.solve();
        Moose::perf_log.pop("solve()","Solve");

        gettimeofday (&solve_end, NULL);  
        double elapsed_time = (static_cast<double>(solve_end.tv_sec  - solve_start.tv_sec) +
                               static_cast<double>(solve_end.tv_usec - solve_start.tv_usec)*1.e-6);

        //This ensures that every processor uses the same times to do solution time adaptivity
        Parallel::broadcast<double>(elapsed_time);

        if(libMesh::processor_id() == 0)
          adaptive_log<<"Solve Elapsed Time: "<<elapsed_time<<std::endl;

        if(converged)
        {
          older_sol_time_vs_dt = old_sol_time_vs_dt;
          old_sol_time_vs_dt = sol_time_vs_dt;
          sol_time_vs_dt = elapsed_time / dt_cur;
          converged = system.nonlinear_solver->converged;
        }

        
        if(converged && adaptivity)
        {
          // Compute the error for each active element
          error_estimator.estimate_error(system, error);
        
          // Flag elements to be refined and coarsened
          mesh_refinement.flag_elements_by_error_fraction (error);
          
          // Perform refinement and coarsening
          mesh_refinement.refine_and_coarsen_elements();
        
          // Reinitialize the equation_systems object for the newly refined
          // mesh. One of the steps in this is project the solution onto the 
          // new mesh
          equation_systems.reinit();

          mesh->boundary_info->build_node_list_from_side_list();
/*
          system.solution->close();
          system.rhs->close();
          system.current_local_solution->close();
          system.old_local_solution->close();
          system.older_local_solution->close();

          Moose::perf_log.push("solve()","Solve");
          // System Solve
          system.solve();
          Moose::perf_log.pop("solve()","Solve");
*/
        }

        converged = system.nonlinear_solver->converged;

        step_rejected = false;

        //Check for error criteria
        if(converged && reject_step_error > 0)
        {
          NumericVector<Number> & time_error = system.get_vector("time_error");
          NumericVector<Number> & old_solution = system.get_vector("old_solution");

          if(t_step != 1)
          {
            //Compute E = .5 * dt * |u - u_old|
            time_error = *system.solution;
            time_error -= old_solution;
            //time_error.abs();
            time_error.scale(0.5 * dt_cur);
            double error_norm = time_error.l2_norm();
            error_norm *= .5 * dt_cur;

            if(error_norm > reject_step_error)
            {
              std::cout<<"Rejecting step based on time error: "<<error_norm<<std::endl;
              
              converged = false;
              step_rejected = true;

              //Set our next guess to half the update of the last step
              time_error.scale(0.5);
              *system.solution = old_solution;
              system.solution->close();
              *system.solution += time_error;
              system.solution->close();

              //Update system.current_local_solution
              system.update();
            }
          }
          else
            old_solution = *system.solution;
        } 

        for(unsigned int var = 0; var < system.n_vars(); var++)
          std::cout<<var<<": "<<system.calculate_norm(*system.rhs,var,DISCRETE_L2)<<std::endl;

        if ( converged && (t_step+1)%interval == 0)
          output_system(equation_systems, file_base, t_step, time, exodus_output, gmv_output, tecplot_output,print_out_info);
        
        // Check for stop condition based upon steady-state check flag:
        if(converged && trans_ss_check == true && time > ss_tmin)
        {
          // Compute new time solution l2_norm
          Real new_time_solution_norm = system.current_local_solution->l2_norm();
          
          // Compute l2_norm relative error
          Real ss_relerr_norm = fabs(new_time_solution_norm - old_time_solution_norm)/new_time_solution_norm;

          // Check current solution relative error norm against steady-state tolerance
          if(ss_relerr_norm < ss_check_tol)
	  {
            keep_going = false;
            std::cout<<"Steady-State Solution Achieved at time: "<<time<<std::endl;
	  }
          else // Keep going
          {
            // Update solution norm for next time step  
            old_time_solution_norm = new_time_solution_norm;
            // Print steady-state relative error norm
            std::cout<<"Steady-State Relative Differential Norm: "<<ss_relerr_norm<<std::endl;
          }
        }
        
        // Check for stop condition based upon number of simulation steps and/or solution end time:
        if(t_step==num_steps)
          keep_going = false;            
        if((time>end_time) || (fabs(time-end_time)<1.e-14))
          keep_going = false;
            
        /*** Compute and print error:  ***
             ExactSolution exact_sol(equation_systems);
             exact_sol.attach_exact_value(ManSol4ADR2exv);
             exact_sol.attach_exact_deriv(ManSol4ADR2exd);
             exact_sol.extra_quadrature_order(4);    
             exact_sol.compute_error("NonlinearSystem","phi");
             std::cout << "   --> L2-Error is: "
             << exact_sol.l2_error("NonlinearSystem","phi")
             << "   H1-Error is: "
             << exact_sol.h1_error("NonlinearSystem","phi")
             << std::endl;
        ***/
            
      } // End of "do while(keep_going)"      
    }    // End of Transient...
    else // Steady-state...
    {
      // Define the refinement loop
      for(unsigned int r_step=0; r_step<=max_r_steps; r_step++)
      {
        system.print_info();
        
        PerfLog solve_only("Solve Only");
        solve_only.push("solve()","Solve");


        Moose::perf_log.push("solve()","Solve");

        system.solve();

        for(unsigned int var = 0; var < system.n_vars(); var++)
         std::cout<<var<<": "<<system.calculate_norm(*system.rhs,var,DISCRETE_L2)<<std::endl;

        Moose::perf_log.pop("solve()","Solve");
        
        solve_only.pop("solve()","Solve");
        solve_only.print_log();
        
        output_system(equation_systems, file_base, 1, 1.0, exodus_output, gmv_output, tecplot_output,print_out_info);
        
        /*** Compute and print error:  ***
             ExactSolution exact_sol(equation_systems);
             exact_sol.attach_exact_value(ManSol4ADR1exv);
             exact_sol.attach_exact_deriv(ManSol4ADR1exd);
             exact_sol.extra_quadrature_order(2);    
             exact_sol.compute_error("NonlinearSystem","phi");
             std::cout << "L2-Error is: "
             << exact_sol.l2_error("NonlinearSystem","phi")
             << "   H1-Error is: "
             << exact_sol.h1_error("NonlinearSystem","phi")
             << std::endl;
        ***/

        // We need to ensure that the mesh is not refined on the last iteration
        // of this loop, since we do not want to refine the mesh unless we are
        // going to solve the equation system for that refined mesh.
        if(r_step != max_r_steps)
        {          
          // Compute the error for each active element
          error_estimator.estimate_error(system, error);
          
          // Flag elements to be refined and coarsened
          mesh_refinement.flag_elements_by_error_fraction (error);
          
          // Perform refinement and coarsening
          mesh_refinement.refine_and_coarsen_elements();
          
          // Tell MOOSE that the Mesh has changed
          Moose::meshChanged();
        }
      } // End of refinement loop
    } // End of steady-state...
  } // Braces are used to force object scope, like in example 2
   
  Moose::perf_log.pop("main()","Falcon");

  // All done.  
  return libMesh::close ();

  Moose::perf_log.print_log();
}
