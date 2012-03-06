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

#include "ApertureAux.h"

template<>
InputParameters validParams<ApertureAux>()
{
	InputParameters params = validParams<AuxKernel>();
     return params;
}

ApertureAux::ApertureAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   _aperture(getMaterialProperty<Real>("aperture"))

{}

Real
ApertureAux::computeValue()
{
  return _aperture[_qp];


}
