#include "CapsuleApertureAux.h"

registerMooseObject("FalconApp", CapsuleApertureAux);


InputParameters
CapsuleApertureAux::validParams(){
    InputParameters params = AuxKernel::validParams();
    params.addClassDescription("AuxKernel for computing fracture apertures based on the distance from the center line of the fracture.");
    params.addRequiredParam<Point>("start_point","starting point of line segment");
    params.addRequiredParam<Point>("end_point","ending point of line segment");
    params.addRequiredParam<Real>("a_max","max initial aperture");
    params.addRequiredParam<Real>("a_min","min initial aperture");
    params.addRequiredParam<Real>("midpoint_of_sigmoid","midpoint of sigmoid curve");
    params.addParam<Real>("slope_at_midpoint",1.0,"slope at midpoint of sigmoid curve");
    return params;
}

CapsuleApertureAux::CapsuleApertureAux(const InputParameters & parameters)
:AuxKernel(parameters),
_x1(getParam<Point>("start_point")),
_x2(getParam<Point>("end_point")),
_aHi(getParam<Real>("a_max")),
_aLo(getParam<Real>("a_min")),
_midpoint_of_sigmoid(getParam<Real>("midpoint_of_sigmoid")),
_slope_at_midpoint(getParam<Real>("slope_at_midpoint"))
{
}

Real CapsuleApertureAux::computeValue(){
    // Obtain the coordinates of the current point
    Point x0 = isNodal()? *_current_node : _q_point[_qp];

    // Compute vector for center line segment
    Point x1x2 = _x2 - _x1;

    // Compute vector between current point and endpoints of
    // center line segment
    Point x1x0 = x0 - _x1;
    Point x2x0 = x0 - _x2;

    Real distance_from_center_line;

    /*
    If the current point does not lie between the two points of
    the line segment, then the distance to the line is the distance
    to the closest endpoint. Else it is the perpendicalar distance
    from the line
    */

    if(x1x2*x1x0 <= 0){
        distance_from_center_line = x1x0.norm();
    }
    else if(x1x2*x2x0 >= 0){
        distance_from_center_line = x2x0.norm();
    }
    else{
        distance_from_center_line = (x1x0.cross(x2x0)).norm()/x1x2.norm();
    }

    Real sigmoid_scale = 1.0/(1.0+std::exp(_slope_at_midpoint*(distance_from_center_line -_midpoint_of_sigmoid)));
    Real fracture_aperture =  (_aHi-_aLo) * sigmoid_scale + _aLo;

    return fracture_aperture;

}
