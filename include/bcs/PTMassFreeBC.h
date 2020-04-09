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

#ifndef PTMASSFREEBC_H
#define PTMASSFREEBC_H

#include "IntegratedBC.h"
#include "Material.h"

class PTMassFreeBC : public IntegratedBC
{
  public:

    PTMassFreeBC(const InputParameters & parameters);

    virtual ~PTMassFreeBC(){}

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();
    virtual Real computeQpOffDiagJacobian(unsigned int jvar);

    bool _has_coupled_temp;

    const MaterialProperty<Real> & _wrho;
    const MaterialProperty<Real> & _wtau;
    const MaterialProperty<Real> & _gfor;
    const MaterialProperty<Real> & _drot;


    const MaterialProperty<RealGradient> & _guvec;
    const MaterialProperty<RealGradient> & _wdmfx;

  private:

    unsigned int _temp_var;

  public:
    static InputParameters validParams();
};
#endif //PTMASSFREEBC_H
