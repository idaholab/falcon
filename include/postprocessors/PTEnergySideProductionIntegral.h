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

#ifndef PTENERGYSIDEPRODUCTIONINTEGRAL_H
#define PTENERGYSIDEPRODUCTIONINTEGRAL_H

#include "SideIntegralVariablePostprocessor.h"
#include "MaterialPropertyInterface.h"
#include "Material.h"

class PTEnergySideProductionIntegral : public SideIntegralVariablePostprocessor
{
  public:

    PTEnergySideProductionIntegral(const InputParameters & parameters);

  protected:

    virtual Real computeQpIntegral();

    const MaterialProperty<RealGradient> & _evelo;

    Real _temp; // temperature of injected fluid

  public:
    static InputParameters validParams();
};

#endif // PTENERGYSIDEPRODUCTIONINTEGRAL_H
