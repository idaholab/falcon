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
#include "Parser.h"

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
void output_system(EquationSystems * equation_systems, std::string file_base, unsigned int t_step, Real time, bool exodus_output, bool gmv_output, bool tecplot_output, bool print_out_info)
{
  OStringStream stream_file_base;
  
  stream_file_base << file_base << "_";
  OSSRealzeroright(stream_file_base,3,0,t_step);

  std::string file_name = stream_file_base.str();

  if(print_out_info)
     std::cout << "   --> Output in file ";
  
  if(exodus_output) 
  {     
    static ExodusII_IO ex_out(equation_systems->get_mesh());
    // The +1 is because Exodus starts timesteps at 1 and we start at 0
    ex_out.write_timestep(file_base + ".e", *equation_systems, t_step+1, time);
    
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
    GMVIO(*Moose::mesh).write_equation_systems(file_name + ".gmv", *equation_systems);
    if(print_out_info)
    {       
      if(exodus_output)
         std::cout << " and ";    
      std::cout << file_name+".gmv";
    }    
  }  
  if(tecplot_output) 
  {     
    TecplotIO(*Moose::mesh).write_equation_systems(file_name + ".plt", *equation_systems);
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

  // Setup Falcon specific settings
  Falcon::registerObjects();
  Moose::init_value = init_value;
  Moose::init_gradient = init_gradient;
  Moose::init_cond = init_cond;
  
  // Braces are used to force object scope, like in example 2
  {
    // Create a GetPot object to parse the command line
    GetPot command_line (argc, argv);

    if (command_line.search("--dump")) 
    {
      Parser p(true);

      // Exit the program without solving any problem
      exit(0);
    }

    std::string input_filename = "";
    if ( command_line.search("-i") )
      input_filename = command_line.next(input_filename);
    else
      mooseError("Must specify an input file using -i");
      

    Parser p(input_filename);
    p.execute();

    GetPot input_file(input_filename);

    
    TransientNonlinearImplicitSystem &system =
      Moose::equation_system->get_system<TransientNonlinearImplicitSystem>("NonlinearSystem");

    TransientExplicitSystem &aux_system =
      Moose::equation_system->get_system<TransientExplicitSystem> ("AuxiliarySystem");
    
    Moose::setSolverDefaults(Moose::equation_system, system, Moose::compute_jacobian_block, Moose::compute_residual);

    if(Moose::execution_type != "Transient" && Moose::execution_type != "Steady")
      mooseError("Must specify either Transient or Steady for Execution/type");

    if(Moose::output_initial)
      output_system(Moose::equation_system, Moose::file_base, 0, 0.0, Moose::exodus_output, Moose::gmv_output, Moose::tecplot_output, Moose::print_out_info);

    
    bool adaptivity = false;

    if(Moose::equation_system->parameters.have_parameter<bool>("adaptivity"))
      adaptivity = Moose::equation_system->parameters.get<bool>("adaptivity");

    unsigned int max_r_steps = 0;

    if(Moose::equation_system->parameters.have_parameter<unsigned int>("max_r_steps"))
      max_r_steps = Moose::equation_system->parameters.get<unsigned int>("max_r_steps");

    bool converged = true;
    bool step_rejected = false;
    
    if(Moose::execution_type == "Transient")
    {
      // Load time steppping prameters for Transient scope
      std::cerr << "Getting Time: " << std::endl;
      double time                    = Moose::equation_system->parameters.get<Real>("time");
      std::cerr << "Got Time" << std::endl;
      double end_time                = Moose::equation_system->parameters.get<Real>("end_time");
      double dt                      = Moose::equation_system->parameters.get<Real>("dt");
      double dtmin                   = Moose::equation_system->parameters.get<Real>("dtmin");
      double dtmax                   = Moose::equation_system->parameters.get<Real>("dtmax");
      int num_steps                  = Moose::equation_system->parameters.get<int>("num_steps");
      int n_startup_steps            = Moose::equation_system->parameters.get<int>("n_startup_steps");
      bool adaptive_time_stepping    = Moose::equation_system->parameters.get<bool>("adaptive_time_stepping");
      bool sol_time_adaptive_time_stepping    = Moose::equation_system->parameters.get<bool>("sol_time_adaptive_time_stepping");
      bool exponential_time_stepping = Moose::equation_system->parameters.get<bool>("exponential_time_stepping");
      bool trans_ss_check            = Moose::equation_system->parameters.get<bool>("trans_ss_check");
      double ss_check_tol            = Moose::equation_system->parameters.get<Real>("ss_check_tol");
      double ss_tmin                 = Moose::equation_system->parameters.get<Real>("ss_tmin");      
      double reject_step_error       = Moose::equation_system->parameters.get<Real>("reject_step_error");
      
      bool keep_going = true;
      int & t_step = Moose::equation_system->parameters.set<int> ("t_step") = 0;
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
          
            Moose::equation_system->parameters.set<Real> ("time")  = time;
            Moose::equation_system->parameters.set<Real> ("dt")    = dt_cur;
            Moose::equation_system->parameters.set<int> ("t_step") = t_step;

            std::cerr << "t_step: " << Moose::equation_system->parameters.get<int>("t_step") << "\n";
            
        
            Kernel::reinitDT();

            Moose::compute_residual(*system.current_local_solution,*system.rhs);
            system.rhs->close();

            initial_residual_norm = system.rhs->l2_norm();
            last_residual_norm = initial_residual_norm;
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
          
                Moose::equation_system->parameters.set<Real> ("time")  = temp_time;
                Moose::equation_system->parameters.set<Real> ("dt")    = temp_dt;
                Moose::equation_system->parameters.set<int> ("t_step") = t_step;
        
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

            Moose::equation_system->parameters.set<Real> ("time")  = time;
            Moose::equation_system->parameters.set<Real> ("dt")    = dt_cur;

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

        Moose::setSolverDefaults(Moose::equation_system, system, Moose::compute_jacobian_block, Moose::compute_residual);

        timeval solve_start, solve_end;
        gettimeofday (&solve_start, NULL);

        std::vector<Real> one_scaling;
                  
        // Reset the scaling to all 1's so we can compute the true residual
        for(unsigned int var = 0; var < system.n_vars(); var++)
          one_scaling.push_back(1.0);

        Kernel::setVarScaling(one_scaling);

        Moose::compute_residual(*system.current_local_solution,*system.rhs);

        std::cout<<"  True Initial Nonlinear Residual: "<<system.rhs->l2_norm()<<std::endl;

        // Set the scaling to manual scaling
        Kernel::setVarScaling(Moose::manual_scaling);

        if(Moose::auto_scaling)
        {
          std::vector<Real> scaling;

          // Compute the new scaling
          for(unsigned int var = 0; var < system.n_vars(); var++)
          {
            Real norm = system.calculate_norm(*system.rhs,var,DISCRETE_L2);

            if(norm != 0)
              scaling.push_back(1.0/norm);
            else
              scaling.push_back(1.0);
          }
          
          Kernel::setVarScaling(scaling);
        }

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
          Moose::error_estimator->estimate_error(system, *Moose::error);
        
          // Flag elements to be refined and coarsened
          Moose::mesh_refinement->flag_elements_by_error_fraction (*Moose::error);
          
          // Perform refinement and coarsening
          Moose::mesh_refinement->refine_and_coarsen_elements();
        
          // Reinitialize the equation_systems object for the newly refined
          // mesh. One of the steps in this is project the solution onto the 
          // new mesh
          Moose::equation_system->reinit();

          Moose::mesh->boundary_info->build_node_list_from_side_list();
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

        if ( converged && (t_step+1)%Moose::interval == 0)
          output_system(Moose::equation_system, Moose::file_base, t_step, time, Moose::exodus_output, Moose::gmv_output, Moose::tecplot_output, Moose::print_out_info);
        
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

        std::vector<Real> one_scaling;
                  
        // Reset the scaling to all 1's so we can compute the true residual
        for(unsigned int var = 0; var < system.n_vars(); var++)
          one_scaling.push_back(1.0);

        Kernel::setVarScaling(one_scaling);

        Moose::compute_residual(*system.current_local_solution,*system.rhs);

        std::cout<<"  True Initial Nonlinear Residual: "<<system.rhs->l2_norm()<<std::endl;

        // Set the scaling to manual scaling
        Kernel::setVarScaling(Moose::manual_scaling);

        if(Moose::auto_scaling)
        {
          std::vector<Real> scaling;

          // Compute the new scaling
          for(unsigned int var = 0; var < system.n_vars(); var++)
          {
            Real norm = system.calculate_norm(*system.rhs,var,DISCRETE_L2);

            if(norm != 0)
              scaling.push_back(1.0/norm);
            else
              scaling.push_back(1.0);
          }
          
          Kernel::setVarScaling(scaling);
        }

        PerfLog solve_only("Solve Only");
        solve_only.push("solve()","Solve");

        Moose::perf_log.push("solve()","Solve");

        system.solve();

#ifdef WITH_PF        
        // Iterate over the materials, cast them and tell them to print their data structures
        std::map<int, Material *>::iterator i;


        for (i = MaterialFactory::instance()->activeMaterialsBegin(0);
             i != MaterialFactory::instance()->activeMaterialsEnd(0); ++i)
        {
          Meso * meso = dynamic_cast<Meso *>(i->second);
          if (meso != NULL)
          {
            meso->printData(*system.current_local_solution);
          }
          
        }
#endif        
        
        for(unsigned int var = 0; var < system.n_vars(); var++)
          std::cout<<var<<": "<<system.calculate_norm(*system.rhs,var,DISCRETE_L2)<<std::endl;

        Moose::perf_log.pop("solve()","Solve");
        
        solve_only.pop("solve()","Solve");
        solve_only.print_log();
        
        output_system(Moose::equation_system, Moose::file_base, r_step+1, r_step+1, Moose::exodus_output, Moose::gmv_output, Moose::tecplot_output, Moose::print_out_info);
        
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
          Moose::error_estimator->estimate_error(system, *Moose::error);
          
          // Flag elements to be refined and coarsened
          Moose::mesh_refinement->flag_elements_by_error_fraction (*Moose::error);
          
          // Perform refinement and coarsening
          Moose::mesh_refinement->refine_and_coarsen_elements();
          
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
