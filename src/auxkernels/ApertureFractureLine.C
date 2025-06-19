#include "ApertureFractureLine.h"

registerMooseObject("FalconApp", ApertureFractureLine);


InputParameters
ApertureFractureLine::validParams(){
    InputParameters params = AuxKernel::validParams();
    //add description 
    params.addRequiredParam<Point>("start_point","starting point of line segment");
    params.addRequiredParam<Point>("end_point","ending point of line segment");
    params.addRequiredParam<Real>("a_max","max initial aperture");
    params.addRequiredParam<Real>("a_min","min initial aperture");
    params.addRequiredParam<Real>("midpoint_of_sigmoid","midpoint of S curve");
    params.addParam<Real>("slope_at_midpoint",1.0,"slope at midpoint of S curve");
    return params;
}

ApertureFractureLine::ApertureFractureLine(const InputParameters & parameters)
:AuxKernel(parameters),
_x1(getParam<Point>("start_point")),
_x2(getParam<Point>("end_point")),
_aHi(getParam<Real>("a_max")),
_aLo(getParam<Real>("a_min")),
_midpoint_of_sigmoid(getParam<Real>("midpoint_of_sigmoid")),
_slope_at_midpoint(getParam<Real>("slope_at_midpoint"))
{
}

Real ApertureFractureLine::computeValue(){
    Real d;
  
    Point x0 = isNodal()? *_current_node : _q_point[_qp];
    Point x1x2 = _x2 - _x1;
    Point x1x0 = x0 - _x1;
    Point x2x0 = x0 - _x2;


    if(x1x2*x1x0 <= 0){
        d = x1x0.norm();
    }
    else if(x1x2*x2x0 >= 0){
        d = x2x0.norm();
    }
    else{
        d= (x1x0.cross(x2x0)).norm()/x1x2.norm();
    }

    // Real sigmoid_scale =
    // Real offset_by_min_value = 

    return (_aHi-_aLo)/(1.0+std::exp(_slope_at_midpoint*(d -_midpoint_of_sigmoid)))+ _aLo;
    
}