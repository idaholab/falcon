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
    //params.addRequiredParam<std::string>("West_Reduced_File_name", "Name of the file containing the West Reduced fracture data.");
    //params.addRequiredParam<std::string>("Fault_File_name", "Name of the file containing the main fault fracture data.");
    params.addRequiredParam<std::vector<int> >("fracture_numbers","The number associated with each of the fractures you would like to include from this file");
    params.addParam<Real>("tolerance", 0.9, "closest_point tolerance");
     return params;
}

FracManMapAux::FracManMapAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
//_west_reduced_file_name(getParam<std::string>("West_Reduced_File_name")),
//_fault_file_name(getParam<std::string>("Fault_File_name")),
_fracture_number_vec(getParam<std::vector<int> >("fracture_numbers")),
_tol(getParam<Real>("tolerance"))

{
    std::string buffer;
    std::vector<int> frac_plane_vector;
    int frac_face_num = 0;
    int frac_file_code = 1;
    int frac_num = 0;
    Real frac_prop1 = 0;
    Real frac_prop2 = 0;
    Real frac_prop3 = 0;
    int frac_set_num = 0;
    int frac_total = 15;
    int frac_vert = 0;
    int frac_vert_num = 0;
    int i = 0;
    unsigned int j = 0;
    int k = 0;
    int line_num = 0;
    int m = 0;
    int vert_count = 0;
    bool vert_end = false;
    std::string vert_end_line;
    bool vert_start = false;
    std::string vert_start_line;
    int vert_start_line_num = 0;
    Real x = 0, y = 0, z = 0;
    
    //resizing _fracture_number vector to the number of values provided in input file
    num_vec_entries = _fracture_number_vec.size();
    
    //opeing file
    //std::ifstream openfile ("East_Reduced.fab");
    //if (openfile.is_open())
    //{
    //    std::cout << "file is open.\n";
    //}
    //else std::cout << "Unable to open file.\n";
    
    //Open East_Reduced.fab file
    std::ifstream openfile("East_Reduced.fab");
    if (!openfile)
        mooseError("Can't open FracMan file ");
    
    vert_end_line = "END FRACTURE";
    vert_start_line = "BEGIN FRACTURE";
    
    //Find the start of the vertices in the FracMan file
    while (vert_start == false)
    {
        getline (openfile,buffer);
        //cout << buffer << "\n";
        if (buffer.compare(0,14,vert_start_line,0,14) == 0)
        {
            vert_start = true;
        }
        else vert_start = false;
        
        line_num = line_num + 1;
    }
    
    //Build fracture array
    while (frac_total > 0)
	{
        openfile >> frac_num >> frac_vert >> frac_set_num >> frac_prop1 >> frac_prop2 >> frac_prop3;
        line_num = line_num +1;
        
        frac_vert = frac_vert + 1;
        while (vert_count < 3)
	    {
            openfile >> frac_vert_num >> x >> y >> z;
            frac_array [i][0] = frac_file_code;
            frac_array [i][1] = frac_num;
            frac_array [i][2] = frac_vert_num;
            frac_array [i][3] = x;
            frac_array [i][4] = y;
            frac_array [i][5] = z;
            vert_count = vert_count + 1;
            i = i +1;
            line_num = line_num +1;
	    }
        while (vert_count < frac_vert)
	    {
            openfile >> frac_vert_num >> x >> y >> z;
            vert_count = vert_count +1;
            line_num = line_num + 1;
	    }
        vert_count = 0;
        frac_total = frac_total - 1;
	}
    openfile.close();
    
    // Rest Variables for West_Reduced.fab
    buffer = "nothing";
    frac_file_code = 2;
    frac_num = 0;
    frac_prop1 = 0;
    frac_prop2 = 0;
    frac_prop3 = 0;
    frac_set_num = 0;
    frac_total = 57;
    frac_vert = 0;
    frac_vert_num = 0;
    line_num = 0;
    vert_count = 0;
    vert_end = false;
    vert_end_line = "END FRACTURE";
    vert_start = false;
    vert_start_line = "BEGIN FRACTURE";
    vert_start_line_num = 0;
    x = 0;
    y = 0;
    z = 0;
    
    //Open West_Reduced.fab file
    std::ifstream west_file("West_Reduced.fab");
    if (!west_file)
    {
        mooseError("Can't open West_Reduced file ");
    }
    
    //Find the start of the vertices in the FracMan file
    while (vert_start == false)
    {
        getline (west_file,buffer);
        //cout << buffer << "\n";
        if (buffer.compare(0,14,vert_start_line,0,14) == 0)
        {
            vert_start = true;
        }
        else vert_start = false;
        
        line_num = line_num + 1;
    }
    
    //Build fracture array
    while (frac_total > 0)
	{
        west_file >> frac_num >> frac_vert >> frac_set_num >> frac_prop1 >> frac_prop2 >> frac_prop3;
        line_num = line_num +1;
        
        frac_vert = frac_vert + 1;
        while (vert_count < 3)
	    {
            west_file >> frac_vert_num >> x >> y >> z;
            frac_array [i][0] = frac_file_code;
            frac_array [i][1] = frac_num;
            frac_array [i][2] = frac_vert_num;
            frac_array [i][3] = x;
            frac_array [i][4] = y;
            frac_array [i][5] = z;
            vert_count = vert_count + 1;
            i = i +1;
            line_num = line_num +1;
	    }
        while (vert_count < frac_vert)
	    {
            west_file >> frac_vert_num >> x >> y >> z;
            vert_count = vert_count +1;
            line_num = line_num + 1;
	    }
        vert_count = 0;
        frac_total = frac_total - 1;
	}
    west_file.close();
    
    // Rest Variables for Fault.fab
    buffer = "nothing";
    frac_file_code = 3;
    frac_num = 0;
    frac_prop1 = 0;
    frac_prop2 = 0;
    frac_prop3 = 0;
    frac_set_num = 0;
    frac_total = 1;
    frac_vert = 0;
    frac_vert_num = 0;
    line_num = 0;
    vert_count = 0;
    vert_end = false;
    vert_end_line = "END TESSFRACTURE";
    vert_start = false;
    vert_start_line = "BEGIN TESSFRACTURE";
    vert_start_line_num = 0;
    x = 0;
    y = 0;
    z = 0;
    
    //Open Fault.fab file
    std::ifstream fault_file("Fault.fab");
    if (!fault_file)
    {
        mooseError("Can't open Fault file ");
    }
    
    //Find the start of the vertices in the FracMan file
    while (vert_start == false)
    {
        getline (fault_file,buffer);
        //cout << buffer << "\n";
        if (buffer.compare(0,18,vert_start_line,0,18) == 0)
        {
            vert_start = true;
        }
        else vert_start = false;
        
        line_num = line_num + 1;
    }
    
    //Build fracture array
    while (frac_total > 0)
	{
        fault_file >> frac_num >> frac_vert >> frac_face_num >> frac_set_num;
        line_num = line_num +1;
        
        frac_vert = frac_vert;
        while (vert_count < 3)
	    {
            fault_file >> frac_vert_num >> x >> y >> z;
            frac_array [i][0] = frac_file_code;
            frac_array [i][1] = frac_num;
            frac_array [i][2] = frac_vert_num;
            frac_array [i][3] = x;
            frac_array [i][4] = y;
            frac_array [i][5] = z;
            vert_count = vert_count + 1;
            i = i +1;
            line_num = line_num +1;
	    }
        while (vert_count < frac_vert)
	    {
            fault_file >> frac_vert_num >> x >> y >> z;
            vert_count = vert_count +1;
            line_num = line_num + 1;
	    }
        vert_count = 0;
        frac_total = frac_total - 1;
	}
    fault_file.close();

}

Real
FracManMapAux::computeValue()
{
    Real _frac_man_map = 0;
    Real xp = 0, yp = 0, zp = 0;
    Plane frac;
    
    for (unsigned int k = 0; k < num_vec_entries; k++)
    {
        int j = _fracture_number_vec[k];
        int i = 0;
    
        //for (j = 0; j < 72; j++)
        //{
        i = (2*j) + (j-3);
        xp = frac_array[i][3];
        yp = frac_array[i][4];
        zp = frac_array[i][5];
        Point p0(xp,yp,zp);
        
        i = (2*j) + (j-2);
        xp = frac_array[i][3];
        yp = frac_array[i][4];
        zp = frac_array[i][5];
        Point p1(xp,yp,zp);
        
        i = (3*j) - 1;
        xp = frac_array[i][3];
        yp = frac_array[i][4];
        zp = frac_array[i][5];
        Point p2(xp,yp,zp);
        
        /*frac_plane_vector[j] = */frac.create_from_three_points(p0,p1,p2);
        
        //for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
        //{
            Point current_point(_current_elem->centroid());
            
            Point point_closest = frac.closest_point(current_point);
            
            bool test = point_closest.absolute_fuzzy_equals(current_point, _tol);
            
        if (test == true)
        {
            _frac_man_map = j;
        //std::cout << "true" << std::endl;
        }
        //}
    //}
    }
    return _frac_man_map;
}
