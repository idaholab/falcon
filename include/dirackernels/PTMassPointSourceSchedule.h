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

#ifndef PTMASSPOINTSOURCESCHEDULE_H
#define PTMASSPOINTSOURCESCHEDULE_H

#include "DiracKernel.h"
#include <iostream>

class PTMassPointSourceSchedule;

template<>
InputParameters validParams<PTMassPointSourceSchedule>();

class PTMassPointSourceSchedule : public DiracKernel
{
  public:
  
    PTMassPointSourceSchedule(const InputParameters & parameters);

    virtual void addPoints();

    virtual Real computeQpResidual();

  protected:

    MooseEnum _isino; // user-input option for injection schedule

    std::string _isfnm; // injection schedule filename

    std::vector<Real> _ptloc; // location of mass injection
    std::vector<Real> _sched; // scheduled times for the stages
    std::vector<Real> _value; // values of mass injection rate

    unsigned int nvalu, nstag, istag;
    Real irate;
    Point _p;
};
#endif //PTMASSPOINTSOURCESCHEDULE_H
