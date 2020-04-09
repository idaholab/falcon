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

#ifndef PTENERGYOUTFLOWBC_H
#define PTENERGYOUTFLOWBC_H

#include "IntegratedBC.h"
#include "Material.h"

class PTEnergyOutFlowBC : public IntegratedBC
{
  public:

    PTEnergyOutFlowBC(const InputParameters & parameters);

    virtual ~PTEnergyOutFlowBC(){}

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    const MaterialProperty<Real> &_thco;

    const MaterialProperty<RealGradient> & _evelo;

  public:
    static InputParameters validParams();
};
#endif //PTENERGYOUTFLOWBC_H
