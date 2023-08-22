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

#ifndef PTMASSSIDEFLUXAVERAGE_H
#define PTMASSSIDEFLUXAVERAGE_H

#include "PTMassSideFluxIntegral.h"

class PTMassSideFluxAverage : public PTMassSideFluxIntegral
{
public:
  PTMassSideFluxAverage(const InputParameters & parameters);
  virtual ~PTMassSideFluxAverage(){}

  virtual void initialize();
  virtual void execute();
  virtual void finalize();
  virtual Real getValue() const;
  virtual void threadJoin(const UserObject & y);

protected:
  Real _volume;

public:
  static InputParameters validParams();
};

#endif // PTMASSSIDEFLUXAVERAGE_H
