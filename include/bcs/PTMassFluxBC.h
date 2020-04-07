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

#ifndef PTMASSFLUXBC_H
#define PTMASSFLUXBC_H

#include "IntegratedBC.h"
#include "Material.h"

class PTMassFluxBC : public IntegratedBC
{
  public:

    PTMassFluxBC(const InputParameters & parameters);

    virtual ~PTMassFluxBC(){}

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    Real _value;

  public:
    static InputParameters validParams();
};
#endif //PTMASSFLUXBC_H
