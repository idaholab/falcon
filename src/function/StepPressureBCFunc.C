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

#include "StepPressureBCFunc.h"

template<>
InputParameters validParams<StepPressureBCFunc>()
{
  InputParameters params=validParams<Function>();
  params.addRequiredParam<Real>("p1","the pressure at time t=0");
  params.addRequiredParam<Real>("p2","the pressure drop rate at the first stage");
  params.addRequiredParam<Real>("p3","the pressure drop rate at the second stage");
  params.addRequiredParam<Real>("p4","the pressure drop rate the the third stage");
  params.addRequiredParam<Real>("t1","the first time stage");
  params.addRequiredParam<Real>("t2","the first time stage");
  params.addRequiredParam<Real>("t3","the first time stage");
  return params;
}

StepPressureBCFunc::StepPressureBCFunc(const std::string & name, InputParameters parameters)
  :Function(name,parameters),
   _p1(getParam<Real>("p1")),
   _p2(getParam<Real>("p2")),
   _p3(getParam<Real>("p3")),
   _p4(getParam<Real>("p4")),
   _t1(getParam<Real>("t1")),
   _t2(getParam<Real>("t2")),
   _t3(getParam<Real>("t3"))
{}


Real
StepPressureBCFunc::value(Real t, const Point & p)
{
  if (t<=_t1)
  {
   return _p1-_p2*t;
  }
  
  else if (t<=_t2)
  {
    return _p1-_p2*_t1-_p3*(t-_t1);
  }
  else if (t<=_t3)
  {
    return _p1-_p2*_t1-_p3*(_t2-_t1)-_p4*(t-_t2);
  }
  else
  {
    return _p1-_p2*_t1-_p3*(_t2-_t1)-_p4*(_t3-_t2);
  }
  
}
  
