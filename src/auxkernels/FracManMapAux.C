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

#include "FracManMapAux.h"

template<>
InputParameters validParams<FracManMapAux>()
{
    InputParameters params = validParams<AuxKernel>();

    MooseEnum metric_conversion_code("no yes", "no");
    params.addParam<MooseEnum>("metric_conversion", metric_conversion_code,
      "Convert to metric? no, yes.");

    MooseEnum normal_component_code("x y z", "x");
    params.addParam<MooseEnum>("normal_component", normal_component_code,
      "Choose the normal component to be calculated:  x, y, or z.");

    MooseEnum output_type_code("fracture_map fracture_normal fracture_tip wellbore debug");
    params.addParam<MooseEnum>("output_type", output_type_code,
      "Choose output:  _frac_man_map, _frac_man_normal<component>, _wellbore.");

    params.addParam<bool>("fracture_rewrite", false,
      "Set to true if you want the fractures to be rewritten every iteration");

    params.addParam<std::vector<Real> >("dfn_tolerance", std::vector<Real>(1),
      "DFN tolerance used to calulate fracture tips");

    params.addParam<std::vector<Real> >("fracture_tolerance", std::vector<Real>(1),
      "closest_point tolerance");

    params.addParam<std::vector<Real> >("refine_schedule", std::vector<Real>(0.0),
      "The times at which each tolerance level changes");

    params.addParam<std::vector<Real> >("wellbore_tolerance", std::vector<Real>(1),
      "wellbore tolerance");

    params.addRequiredParam<std::vector<int> >("fracture_numbers",
      "List the ID number of the fractures to be included in the simulation");

    params.addRequiredParam<std::vector<std::string> >("file_names",
      "List the FracMan files to be loaded");

    params.addRequiredParam<std::vector<Real> >("coordinate_shift",
      "Enter the x, y, and z elements of the coordinate shift (ft)");

    params.addRequiredParam<std::vector<Real> >("dfn_box_max_adjustment",
      "Enter the x, y, and z displacements.");

    params.addRequiredParam<std::vector<Real> >("dfn_box_min_adjustment",
      "Enter the x, y, and z displacements.");

    params.addRequiredParam<std::vector<Real> >("dfn_offset",
      "Enter the x, y, and z displacements of the dfn offset (m)");

    params.addRequiredParam<std::vector<Real> >("dfn_size",
      "Enter the x, y, and z lengths of the DFN (ft)");

    params.addRequiredParam<std::vector<Real> >("mesh_size",
      "Enter the x, y, and z lengths of the mesh (m)");

    params.addRequiredParam<std::vector<Real> >("wellbore_location",
      "Enter the x and y elements of the wellbore location (m)");

    return params;
}

FracManMapAux::FracManMapAux(const InputParameters & parameters):
    AuxKernel(parameters),
    _fracture_rewrite(getParam<bool>("fracture_rewrite")),
    _metric_conversion(getParam<MooseEnum>("metric_conversion")),
    _normal_component(getParam<MooseEnum>("normal_component")),
    _output_type(getParam<MooseEnum>("output_type")),
    _fracture_number_vec(getParam<std::vector<int> >("fracture_numbers")),
    _coordinate_shift(getParam<std::vector<Real> >("coordinate_shift")),
    _dfn_box_max_adjustment(getParam<std::vector<Real> >("dfn_box_max_adjustment")),
    _dfn_box_min_adjustment(getParam<std::vector<Real> >("dfn_box_min_adjustment")),
    _dfn_offset(getParam<std::vector<Real> >("dfn_offset")),
    _dfn_size(getParam<std::vector<Real> >("dfn_size")),
    _dfn_tol(getParam<std::vector<Real> >("dfn_tolerance")),
    _mesh_size(getParam<std::vector<Real> >("mesh_size")),
    _refine_schedule(getParam<std::vector<Real> >("refine_schedule")),
    _fracture_tol(getParam<std::vector<Real> >("fracture_tolerance")),
    _wellbore_location(getParam<std::vector<Real> >("wellbore_location")),
    _wellbore_tol(getParam<std::vector<Real> >("wellbore_tolerance")),
    _file_names(getParam<std::vector<std::string> > ("file_names"))
{
  bool found_fracture_vertices_start_line = false;
  bool found_number_of_fractures_line = false;
  bool found_tessfracture_vertices_start_line = false;

  Plane tessfrac;

  Real conversion_factor_length = 1.;
  Real dfn_shift_x = 0.;
  Real dfn_shift_y = 0.;
  Real dfn_shift_z = 0.;
  Real extra = 0.;
  Real nx = 0.;
  Real ny = 0.;
  Real nz = 0.;
  Real txp = 0.;
  Real typ = 0.;
  Real tzp = 0.;
  Real x = 0.;
  Real y = 0.;
  Real z = 0.;

  std::string buffer = "None";
  std::string file_name = "None";
  std::string fracture_vertices_start_line_key = "BEGIN FRACTURE";
  std::string number_of_fractures_start_line_key = "Scale";
  std::string tessfracture_vertices_start_line_key = "BEGIN TESSFRACTURE";

  std::vector<unsigned int> number_of_fractures;
  std::vector<unsigned int> number_of_tessfractures;

  unsigned int current_number_of_fractures = 0;
  unsigned int normal_index = 0;
  unsigned int number_of_files = 0;
  unsigned int number_of_fractures_in_file = 0;
  unsigned int number_of_tessfractures_in_file = 0;
  unsigned int number_of_vertices = 0;
  unsigned int number_of_vertices_in_fracture = 0;
  unsigned int point_index = 0;
  unsigned int vertice_index = 0;

  //Determine the size of num_vec_entries for indexing purposes
  num_vec_entries = _fracture_number_vec.size();

  //Determine the size of _file_names for indexing purposes
  number_of_files = _file_names.size();

  //Resize number_of_fractures and number_of_tessfractures based on user input for storage purposes
  number_of_fractures.resize(number_of_files);
  number_of_tessfractures.resize(number_of_files);

  //Determine if dfn units need to be converted to metric units
  //Set variable conversion_factor_length to 1 m/m if no metric converison is required
  //Set variable conversion_factor_lenght to 0.3048 m/ft if a metric conversion is required
  if (_metric_conversion == 0)
    conversion_factor_length = 1;
  else
    conversion_factor_length = 0.3048;

  //Calculate the DFN dimension shift
  dfn_shift_x = (_mesh_size[0] - conversion_factor_length * _dfn_size[0]) / 2 + _dfn_offset[0];
  dfn_shift_y = (_mesh_size[1] - conversion_factor_length * _dfn_size[1]) / 2 + _dfn_offset[1];
  dfn_shift_z = (_mesh_size[2] - conversion_factor_length * _dfn_size[2]) / 2 + _dfn_offset[2];

  //Calculate the DFN box boundaries
  dfn_box_x_max = dfn_shift_x + conversion_factor_length * _dfn_size[0] + _dfn_box_max_adjustment[0];
  dfn_box_y_max = dfn_shift_y + conversion_factor_length * _dfn_size[1] + _dfn_box_max_adjustment[1];
  dfn_box_z_max = dfn_shift_z + conversion_factor_length * _dfn_size[2] + _dfn_box_max_adjustment[2];

  dfn_box_x_min = dfn_shift_x + _dfn_box_min_adjustment[0];
  dfn_box_y_min = dfn_shift_y + _dfn_box_min_adjustment[1];
  dfn_box_z_min = dfn_shift_z + _dfn_box_min_adjustment[2];

  //Check that dfn_box_max_adjustment is greater than dfn_box_min_adjustment
  if (dfn_box_x_max <= dfn_box_x_min)
    mooseError("FracManMapAux:  dfn_box_x_max and dfn_box_y_min contain illegal values");

  if (dfn_box_y_max <= dfn_box_y_min)
    mooseError("FracManMapAux:  dfn_box_x_max and dfn_box_y_min contain illegal values");

  if (dfn_box_z_max <= dfn_box_z_min)
    mooseError("FracManMapAux:  dfn_box_x_max and dfn_box_y_min contain illegal values");

  // Open FracMan files to read in fracture vertices
  for (unsigned int i = 0; i < number_of_files; i++)
  {
    //Set file_name to the file that is to be loaded so that it can function with ifstream
    file_name = _file_names [i];

    //Load the file for reading
    std::ifstream fracman_file(file_name.c_str());

    //Check that the file loaded correctly
    if (!fracman_file)
      mooseError("FracManMapAux:  Error opening FracMan file.");

    //Find the number of fractures and tessfractures in the file
    while (found_number_of_fractures_line == false)
    {
      //Read a line from the file so it can be compared with the key
      getline(fracman_file,buffer);

      if (buffer.compare(4,5,number_of_fractures_start_line_key,0,5) == 0)
      {
        found_number_of_fractures_line = true;

        //Read the next line to determine the number of fractures in the file then store that value
        getline(fracman_file,buffer,'=');
        fracman_file >> number_of_fractures_in_file;
        number_of_fractures[i] = number_of_fractures_in_file;

        //Read the next line to determine the number of tessfractures in the file then store that value
        getline(fracman_file,buffer,'=');
        fracman_file >> number_of_tessfractures_in_file;
        number_of_tessfractures[i] = number_of_tessfractures_in_file;
      }
    }

    //Reset variable for next iteration
    found_number_of_fractures_line = false;

    //Resize the fracture vertices vectors in preperation to store values read from the FracMan file
    current_number_of_fractures = current_number_of_fractures + number_of_fractures[i] + number_of_tessfractures[i];
    number_of_vertices = current_number_of_fractures * 3;
    fracture_vertices_x.resize(number_of_vertices);
    fracture_vertices_y.resize(number_of_vertices);
    fracture_vertices_z.resize(number_of_vertices);

    //Resize the fracture normal vectors in preperation to store values read from the FracMan file
    fracture_normal_x.resize(current_number_of_fractures);
    fracture_normal_y.resize(current_number_of_fractures);
    fracture_normal_z.resize(current_number_of_fractures);

    //Check if the file has fractures and if so begin the process of extracting their vertices
    if (number_of_fractures[i] > 0)
    {
      //Look for the start of the fracture vertices in the file
      while (found_fracture_vertices_start_line == false)
      {
        //Read a line from the file so it can be compared with the key
        getline(fracman_file,buffer);

        if (buffer.compare(0,14,fracture_vertices_start_line_key,0,14) == 0)
          found_fracture_vertices_start_line = true;
      }

      //Reset variable for next iteration
      found_fracture_vertices_start_line = false;

      //Read and store the fracture vertices
      for (unsigned int j = 0; j < number_of_fractures[i]; j++)
      {
        //Read the first line of the fracture vertice block
        fracman_file >> extra >> number_of_vertices_in_fracture >> extra >> extra >> extra >> extra;

        //Read and store the first 3 vertices of the fracture
        for (unsigned int k = 0; k < 3; k++)
        {
          fracman_file >> extra >> x >> y >> z;

          fracture_vertices_x[vertice_index] = (x + _coordinate_shift[0]) * conversion_factor_length + dfn_shift_x;
          fracture_vertices_y[vertice_index] = (y + _coordinate_shift[1]) * conversion_factor_length + dfn_shift_y;
          fracture_vertices_z[vertice_index] = (z + _coordinate_shift[2]) * conversion_factor_length + dfn_shift_z;
          vertice_index = vertice_index + 1;
        }

        //Read and discard the remaning vertices of the fracture
        for (unsigned int l = 3; l < number_of_vertices_in_fracture; l++)
          fracman_file >> extra >> x >> y >> z;

        //Read and store the fracture plane normal values
        fracman_file >> extra >> nx >> ny >> nz;

        fracture_normal_x[normal_index] = nx;
        fracture_normal_y[normal_index] = ny;
        fracture_normal_z[normal_index] = nz;
        normal_index = normal_index + 1;
      }
    }
    //Check if the file has tessfractures and if so begin the process of extracting their vertices
    if (number_of_tessfractures[i] > 0)
    {
      //Look for the start of the fracture vertices in the file
      while (found_tessfracture_vertices_start_line == false)
      {
        //Read a line from the file so it can be compared with the key
        getline(fracman_file,buffer);

        if (buffer.compare(0,18,tessfracture_vertices_start_line_key,0,18) == 0)
          found_tessfracture_vertices_start_line = true;
      }

      //Reset variable for next iteration
      found_tessfracture_vertices_start_line = false;

      //Read and store the fracture vertices
      for (unsigned int j = 0; j < number_of_tessfractures[i]; j++)
      {
        //Read the first line of the fracture vertice block
        fracman_file >> extra >> number_of_vertices_in_fracture >> extra >> extra;

        //Read and store the first 3 vertices of the tessfracture
        for (unsigned int k = 0; k < 3; k++)
        {
          fracman_file >> extra >> x >> y >> z;

          fracture_vertices_x[vertice_index] = (x + _coordinate_shift[0]) * conversion_factor_length + dfn_shift_x;
          fracture_vertices_y[vertice_index] = (y + _coordinate_shift[1]) * conversion_factor_length + dfn_shift_y;
          fracture_vertices_z[vertice_index] = (z + _coordinate_shift[2]) * conversion_factor_length + dfn_shift_z;
          vertice_index = vertice_index + 1;
        }

        //Read and discard the remaning vertices of the tessfracture
        for (unsigned int l = 3; l < number_of_vertices_in_fracture; l++)
          fracman_file >> extra >> x >> y >> z;

        //Tessfractures do not have fracture plane normal values so the last line will be discarded
        fracman_file >> extra >> extra >> extra >> extra >> extra >> extra >> extra >> extra >> extra >> extra;

        //Create 3 points so that a plane for the tessfracture can be created
        point_index = vertice_index - 3;
        txp = fracture_vertices_x [point_index];
        typ = fracture_vertices_y [point_index];
        tzp = fracture_vertices_z [point_index];
        Point tp0(txp,typ,tzp);

        point_index = vertice_index - 2;
        txp = fracture_vertices_x [point_index];
        typ = fracture_vertices_y [point_index];
        tzp = fracture_vertices_z [point_index];
        Point tp1(txp,typ,tzp);

        point_index = vertice_index - 1;
        txp = fracture_vertices_x [point_index];
        typ = fracture_vertices_y [point_index];
        tzp = fracture_vertices_z [point_index];
        Point tp2(txp,typ,tzp);

        //Create a plan from 3 points so that a unit normal vector can be created
        tessfrac.create_from_three_points(tp0,tp1,tp2);

        //Create a point that is a unit normal of tessfrac and store the results
        Point tessfrac_normal = tessfrac.unit_normal(tp0);
        fracture_normal_x[normal_index] = tessfrac_normal.operator()(0);
        fracture_normal_y[normal_index] = tessfrac_normal.operator()(1);
        fracture_normal_z[normal_index] = tessfrac_normal.operator()(2);
        normal_index = normal_index + 1;
      }
    }
    fracman_file.close();
  }
}

Real
FracManMapAux::computeValue()
{
    Real current_point_x = 0.;
    Real current_point_y = 0.;
    Real current_point_z = 0.;
    Real dfn_box_x_max_tol_min = 0.;
    Real dfn_box_x_max_tol_max = 0.;
    Real dfn_box_x_min_tol_min = 0.;
    Real dfn_box_x_min_tol_max = 0.;
    Real dfn_box_y_max_tol_min = 0.;
    Real dfn_box_y_max_tol_max = 0.;
    Real dfn_box_y_min_tol_min = 0.;
    Real dfn_box_y_min_tol_max = 0.;
    Real dfn_box_z_max_tol_min = 0.;
    Real dfn_box_z_max_tol_max = 0.;
    Real dfn_box_z_min_tol_min = 0.;
    Real dfn_box_z_min_tol_max = 0.;
    Plane frac;
    Real _frac_man_map = 0.;
    Real _frac_man_normal_x = 0.;
    Real _frac_man_normal_y = 0.;
    Real _frac_man_normal_z = 0.;
    Real frac_man_tip = 0.;
    Real f_tolerance = 0.;
    Real d_tolerance = 0.;
    Real w_tolerance = 0.;
    Real _wellbore = 0.;
    Real xp = 0.;
    Real yp = 0.;
    Real zp = 0.;

    if ( _fracture_rewrite == false)
      _frac_man_map = _u[_qp];
    else
      _frac_man_map = 0;

    //Determine which fracture tolerance and wellbore tolerance to use based on user defined schedule
    _nstages = _refine_schedule.size();

    if (_nstages > 1)
    {
      for (unsigned int a = 0; a < _nstages; ++a)
      {
        if(_t >= _refine_schedule[a] and _t < _refine_schedule[a+1])
        {
          f_tolerance = _fracture_tol[a];
          d_tolerance = _dfn_tol[a];
          w_tolerance = _wellbore_tol[a];
          break;
        }
      }
      if (_t >= _refine_schedule[_nstages - 1])
      {
        f_tolerance = _fracture_tol[_nstages - 1];
        d_tolerance = _dfn_tol[_nstages - 1];
        w_tolerance = _wellbore_tol[_nstages - 1];
      }
    }
    else
    {
      f_tolerance = _fracture_tol[0];
      d_tolerance = _dfn_tol[0];
      w_tolerance = _wellbore_tol[0];
    }

    //Calculate dfn box edge tolerance ranges to determine fracture tips

    //Calculate x components
    dfn_box_x_max_tol_min = dfn_box_x_max - d_tolerance;
    dfn_box_x_max_tol_max = dfn_box_x_max + d_tolerance;

    dfn_box_x_min_tol_min = dfn_box_x_min - d_tolerance;
    dfn_box_x_min_tol_max = dfn_box_x_min + d_tolerance;

    //Calculate y components
    dfn_box_y_max_tol_min = dfn_box_y_max - d_tolerance;
    dfn_box_y_max_tol_max = dfn_box_y_max + d_tolerance;

    dfn_box_y_min_tol_min = dfn_box_y_min - d_tolerance;
    dfn_box_y_min_tol_max = dfn_box_y_min + d_tolerance;

    //Calculate z components
    dfn_box_z_max_tol_min = dfn_box_z_max - d_tolerance;
    dfn_box_z_max_tol_max = dfn_box_z_max + d_tolerance;

    dfn_box_z_min_tol_min = dfn_box_z_min - d_tolerance;
    dfn_box_z_min_tol_max = dfn_box_z_min + d_tolerance;

    //Assign fracture codes to elements containing fractures
    for (unsigned int k = 0; k < num_vec_entries; k++)
    {
      int j = _fracture_number_vec[k];
      int i = 0;
      int m = j - 1;

      //Create points for each vertice so that a plane can be calculated
      i = (2*j) + (j-3);
      xp = fracture_vertices_x [i];
      yp = fracture_vertices_y [i];
      zp = fracture_vertices_z [i];
      Point p0(xp,yp,zp);

      i = (2*j) + (j-2);
      xp = fracture_vertices_x [i];
      yp = fracture_vertices_y [i];
      zp = fracture_vertices_z [i];
      Point p1(xp,yp,zp);

      i = (3*j) - 1;
      xp = fracture_vertices_x [i];
      yp = fracture_vertices_y [i];
      zp = fracture_vertices_z [i];
      Point p2(xp,yp,zp);

      //Create the plane and check if the current element contains the fracture
      frac.create_from_three_points(p0,p1,p2);

      Point current_point(_current_elem->centroid());

      //Expand x, y, and z coordinates that make up curent_point to determine if the current point resides in the dfn box
      current_point_x = current_point.operator()(0);
      current_point_y = current_point.operator()(1);
      current_point_z = current_point.operator()(2);

      Point point_closest = frac.closest_point(current_point);

      bool test = point_closest.absolute_fuzzy_equals(current_point, f_tolerance);

      //If the element contains the fracture test if the element resides withing the dfn box
      if (test == true)
      {
        //If the element resides within the x dimensions of the dfn box test if the element resides within the y dimensions of the dfn box
        if (current_point_x >= dfn_box_x_min && current_point_x <= dfn_box_x_max)
        {
          //If the element resides within the y dimension of the dfn box test if the element resides within the z dimension of the dfn box
          if (current_point_y >= dfn_box_y_min && current_point_y <= dfn_box_y_max)
          {
            //If the element resides with the z dimension of the dfn box assign the element the fracture code and normal components
            if (current_point_z >= dfn_box_z_min && current_point_z <= dfn_box_z_max)
            {
              //Assign fracture code to element
              _frac_man_map = j;

              //Assign fracture normal components to element
              _frac_man_normal_x = fracture_normal_x[m];
              _frac_man_normal_y = fracture_normal_y[m];
              _frac_man_normal_z = fracture_normal_z[m];
              //std::cout << "true" << std::endl;

              //Determine if the element contains the fracture tip

              //Check if the current point resides inside the dfn sides
              if (current_point_x >= dfn_box_x_max_tol_min && current_point_x <= dfn_box_x_max_tol_max)
                frac_man_tip = j;
              else if (current_point_x >= dfn_box_x_min_tol_min && current_point_x <= dfn_box_x_min_tol_max)
                frac_man_tip = j;
              else if (current_point_y >= dfn_box_y_max_tol_min && current_point_y <= dfn_box_y_max_tol_max)
                frac_man_tip = j;
              else if (current_point_y >= dfn_box_y_min_tol_min && current_point_y <= dfn_box_y_min_tol_max)
                frac_man_tip = j;
              else if (current_point_z >= dfn_box_z_max_tol_min && current_point_z <= dfn_box_z_max_tol_max)
                frac_man_tip = j;
              else if (current_point_z >= dfn_box_z_min_tol_min && current_point_z <= dfn_box_z_min_tol_max)
                frac_man_tip = j;
              else
                frac_man_tip = 0;
            }
          }
        }
      }
    }

    //Calculate the Wellbore box boundaries
    wellbore_box_x_max = _wellbore_location[0] + (w_tolerance/2);
    wellbore_box_y_max = _wellbore_location[1] + (w_tolerance/2);

    wellbore_box_x_min = _wellbore_location[0] - (w_tolerance/2);
    wellbore_box_y_min = _wellbore_location[1] - (w_tolerance/2);

    //Test if the elemetn is within the x coordinates of the wellbore
    if (current_point_x >= wellbore_box_x_min && current_point_x <= wellbore_box_x_max)
    {
      //Test if the elemtn is within the y coordinates of the wellbore
      if (current_point_y >= wellbore_box_y_min && current_point_y <= wellbore_box_y_max)
        _wellbore = 1;
    }

    //Run through a series of logic statements to output the required information based on user input
    if (_output_type == 0)
      return _frac_man_map;
    else if (_output_type == 1)
    {
      if (_normal_component == 0)
        return _frac_man_normal_x;
      else if (_normal_component == 1)
        return _frac_man_normal_y;
      else if (_normal_component == 2)
        return _frac_man_normal_z;
      else
        mooseError("FracManMapAux:  component is out of bounds.  Must be either x, y, z.");
    }
    else if (_output_type == 2)
      return frac_man_tip;
    else if (_output_type == 3)
      return _wellbore;
    else if (_output_type == 4)
      return _nstages;
    else
      mooseError("FracManMapAux:  output type is out of bounds. Must be either fracture_map, fracture_normal, fracture_tip, or wellbore.");
}
