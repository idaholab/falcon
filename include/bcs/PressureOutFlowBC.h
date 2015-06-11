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

//******************************************************************************
/*!
  \file    include/bcs/PressurePressureOutFlowBC.h
  \author  Yidong Xia
  \date    October 2014
  \brief   Pressure outflow B.C. in mass balance
 */
//******************************************************************************

#ifndef PRESSUREOUTFLOWBC_H
#define PRESSUREOUTFLOWBC_H

#include "IntegratedBC.h"
#include "Material.h"
#include "libmesh/vector_value.h"

//Forward Declarations
class PressureOutFlowBC;

template<>
InputParameters validParams<PressureOutFlowBC>();

class PressureOutFlowBC : public IntegratedBC
{
  public:

    PressureOutFlowBC(const std::string & name, InputParameters parameters);

    virtual ~PressureOutFlowBC(){}

  protected:

    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();

    const MaterialProperty<Real> & _tau_water;
};

#endif
