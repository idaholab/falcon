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

Descriptions: calculate P-T based volume-average mass flux on a side

----------------------------------------------------------------*/

#include "PTMassSideFluxAverage.h"

registerMooseObject("FalconApp", PTMassSideFluxAverage);

InputParameters PTMassSideFluxAverage::validParams()
{
  InputParameters params = PTMassSideFluxIntegral::validParams();
  return params;
}


/*******************************************************************************
Routine: PTMassSideFluxAverage --- constructor
Authors: Yidong Xia
*******************************************************************************/
PTMassSideFluxAverage::PTMassSideFluxAverage(const InputParameters & parameters) :
    PTMassSideFluxIntegral(parameters),
    _volume(0)
{}


/*******************************************************************************
Routine: PTMassSideFluxAverage --- initialize
Authors: Yidong Xia
*******************************************************************************/
void
PTMassSideFluxAverage::initialize()
{
  PTMassSideFluxIntegral::initialize();
  _volume = 0;
}


/*******************************************************************************
Routine: PTMassSideFluxAverage --- execute
Authors: Yidong Xia
*******************************************************************************/
void
PTMassSideFluxAverage::execute()
{
  PTMassSideFluxIntegral::execute();
  _volume += _current_side_volume;
}


void
PTMassSideFluxAverage::finalize()
{
  PTMassSideFluxIntegral::finalize();
  gatherSum(_volume);
}


/*******************************************************************************
Routine: PTMassSideFluxAverage --- getValue
Authors: Yidong Xia
*******************************************************************************/
Real
PTMassSideFluxAverage::getValue() const
{
  return _integral_value / _volume;
}


/*******************************************************************************
Routine: PTMassSideFluxAverage --- threadJoin
Authors: Yidong Xia
*******************************************************************************/
void
PTMassSideFluxAverage::threadJoin(const UserObject & y)
{
  PTMassSideFluxIntegral::threadJoin(y);
  const PTMassSideFluxAverage & pps = static_cast<const PTMassSideFluxAverage &>(y);
  _volume += pps._volume;
}
