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

#ifndef PTENERGYPOINTSOURCE_H
#define PTENERGYPOINTSOURCE_H

#include "DiracKernel.h"

class PTEnergyPointSource;

template<>
InputParameters validParams<PTEnergyPointSource>();

class PTEnergyPointSource : public DiracKernel
{
  public:
  
    PTEnergyPointSource(const std::string & name, InputParameters parameters);

    virtual void addPoints();

    virtual Real computeQpResidual();

  protected:

    std::vector<Real> _ptloc; // location of energy source
    std::vector<Real> _sched; // scheduled time stages
    std::vector<Real> _value; // mass injection rates
    std::vector<Real> _temps; // temperature values of injected fluid

    MaterialProperty<Real> & _wsph;

    unsigned int nvalu, ntemp, nstag, istag;
    Real irate;
    Point _p;
};
#endif //PTENERGYPOINTSOURCE_H
