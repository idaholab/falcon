/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c)     2012 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

/*----------------------------------------------------------------

Contributors: Yidong Xia (INL)

Descriptions: scheduled energy point source

----------------------------------------------------------------*/

#include "PTEnergyPointSource.h"

registerMooseObject("FalconApp", PTEnergyPointSource);

template<>
InputParameters validParams<PTEnergyPointSource>()
{
  InputParameters params = validParams<DiracKernel>();

  params.addRequiredParam<std::vector<Real> >(
  "point",
  "The x,y,z coordinates of the mass injection point");

  params.addParam<std::vector<Real> >(
  "schedule", std::vector<Real>(0.0),
  "The times each injection stage starts");

  params.addParam<std::vector<Real> >(
  "value", std::vector<Real>(0.0),
  "The mass injection rate at each stage");

  params.addParam<std::vector<Real> >(
  "injection_temperature", std::vector<Real>(0.0),
  "Temperature values of injected fluid");

  return params;
}


/*******************************************************************************
Routine: PTEnergyPointSource --- constructor
Authors: Yidong Xia
*******************************************************************************/
PTEnergyPointSource::PTEnergyPointSource(const InputParameters & parameters):
  DiracKernel(parameters),
  _ptloc(getParam<std::vector<Real> >("point")),
  _sched(getParam<std::vector<Real> >("schedule")),
  _value(getParam<std::vector<Real> >("value")),
  _temps(getParam<std::vector<Real> >("injection_temperature")),
  _wsph(getMaterialProperty<Real>("specific_heat_water"))
{
  _p(0) = _ptloc[0];

  if (_ptloc.size() > 1)
  {
    _p(1) = _ptloc[1];

    if (_ptloc.size() > 2)
    {
      _p(2) = _ptloc[2];
    }
  }

  nvalu = _value.size();
  ntemp = _temps.size();
  nstag = _sched.size();

  if (nvalu == 0)
    mooseError("Need to provide at least one value for injection rate");
  if (ntemp == 0)
    mooseError("Need to provide at least one injection temperature value");
  if (nvalu != nstag)
      mooseError("Number of stages should equal to that of injection rate");
  if (ntemp != nstag)
      mooseError("Number of stages should equal to that of injection temperature");
}


/*******************************************************************************
Routine: addPoints
Authors: Yidong Xia
*******************************************************************************/
void
PTEnergyPointSource::
addPoints()
{
  addPoint(_p);
}


/*******************************************************************************
Routine: computeQpResidual
Authors: Yidong Xia
*******************************************************************************/
Real
PTEnergyPointSource::
computeQpResidual()
{
  if (nstag > 1)
  {
    for (istag=0; istag<nstag; istag++)
      if (_t >= _sched[istag] && _t < _sched[istag+1])
      {
        irate = _wsph[_qp] * (_temps[istag]-_u[_qp]) * _value[istag];
        break;
      }

    if (_t >= _sched[nstag-1])
      irate = _wsph[_qp] * (_temps[nstag-1]-_u[_qp]) * _value[nstag-1];
  }
  else
    irate = _wsph[_qp] * (_temps[0]-_u[_qp]) * _value[0];

  return -_test[_i][_qp]*irate;
}
