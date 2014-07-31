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

#include "FracTipLocationAux.h"

template<>
InputParameters validParams<FracTipLocationAux>()
{
    InputParameters params = validParams<AuxKernel>();

    // parms.addRequiredParam<std::vector<Real> >("dfn_
    params.addRequiredCoupledVar("fracture_map", "AuxVariable that maps out the fracture network");
    params.addRequiredParam<std::vector<Real> >("wellbore_location", "Enter the x, y coordinates of the wellbore (m), note that the wellbore is assumed to be vertical");
    

   return params;
}

FracTipLocationAux::FracTipLocationAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
_fracture_map(coupledValue("fracture_map")),
_wellbore_location(getParam<std::vector<Real> >("wellbore_location"))

{}



Real
FracTipLocationAux::computeValue()
{
    //Initialize variables

    Real current_point_x = 0;
    Real current_point_y = 0;
    Real distance = 0;
    Real wellbore_location_x = 0;
    Real wellbore_location_y = 0;
    Real x_term_squared = 0;
    Real y_term_squared = 0;

    //Grab the center of the current element for use with the distance formula

    Point current_point(_current_elem->centroid());

    //Extract the x and y coordinates from current_point for use with the distance formula
    //Note that the z coordinate is ignored since the wellbore is vertical
    current_point_x = current_point.operator()(0);
    current_point_y = current_point.operator()(1);

    //Extract the x and y coordinates from _wellbore_location for use with the distance formula
    //Note that the z coordinate is ignored since the wellbore is vertical

    wellbore_location_x = _wellbore_location[0];
    wellbore_location_y = _wellbore_location[1];

    //Apply the distance formula to current_point and _wellbore_location

    //Calculate (x2-x1)^2 and (y2-y1)^2 for the distance formula
    x_term_squared = (current_point_x - wellbore_location_x) * (current_point_x - wellbore_location_x);
    y_term_squared = (current_point_y - wellbore_location_y) * (current_point_y - wellbore_location_y);

    //Calculate the squre root term of the distance formula

    distance = std::pow((x_term_squared + y_term_squared), 0.5);

    //If the the element contains a fracture (fracture_map > 0) then assign the value obtained from the distance formula
    if (_fracture_map[_qp] == 0)
    {
      
        return 0;
    }
    
    else if (_fracture_map[_qp] > 0)
    {
      
        return distance;
    }

    else
    {
        mooseError("Encounterd a fracture_map ID less than zero");
        return 0;
    }
    
}

