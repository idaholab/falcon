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

#ifndef PTENERGYPOINTSOURCESCHEDULE_H
#define PTENERGYPOINTSOURCESCHEDULE_H

#include "DiracKernel.h"

class PTEnergyPointSourceSchedule;

template<>
InputParameters validParams<PTEnergyPointSourceSchedule>();

class PTEnergyPointSourceSchedule : public DiracKernel
{
  public:

    PTEnergyPointSourceSchedule(const InputParameters & parameters);

    virtual void addPoints();

    virtual Real computeQpResidual();

  protected:

    MooseEnum _isino; // user-input option for injection schedule

    std::string _isfnm; // injection schedule filename

    std::vector<Real> _ptloc; // location of energy source
    std::vector<Real> _sched; // scheduled time stages
    std::vector<Real> _value; // mass injection rates
    std::vector<Real> _temps; // temperature values of injected fluid

    const MaterialProperty<Real> & _wsph;

    unsigned int nvalu, ntemp, nstag, istag;
    Real irate;
    Point _p;
};
#endif //PTENERGYPOINTSOURCESCHEDULE_H
