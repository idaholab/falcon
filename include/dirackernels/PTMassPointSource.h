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

#ifndef PTMASSPOINTSOURCE_H
#define PTMASSPOINTSOURCE_H

#include "DiracKernel.h"

class PTMassPointSource : public DiracKernel
{
  public:

    PTMassPointSource(const InputParameters & parameters);

    virtual void addPoints();

    virtual Real computeQpResidual();

  protected:

    std::vector<Real> _ptloc; // location of mass injection
    std::vector<Real> _sched; // scheduled times for the stages
    std::vector<Real> _value; // values of mass injection rate

    unsigned int nvalu, nstag, istag;
    Real irate;
    Point _p;

  public:
    static InputParameters validParams();
};
#endif //PTMASSPOINTSOURCE_H
