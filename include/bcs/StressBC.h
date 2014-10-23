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

//***************************************************************
/*!
  \file    include/bcs/StressBC.h
  \author  Yidong Xia 
  \date    October 2014
  \brief   Specify external stress
 */
//***************************************************************

#ifndef STRESSBC_H
#define STRESSBC_H

#include "IntegratedBC.h"

//Forward Declarations
class StressBC;

template<>
InputParameters validParams<StressBC>();

class StressBC : public IntegratedBC
{
  public:
  
    StressBC(const std::string & name, InputParameters parameters);
  
  protected:

    virtual Real computeQpResidual();

   /*! direction
    */
    unsigned int _component;

   /*! Stress
    */
    Real _pressure;

   /*! Biot effective stress coefficient
    */
    Real _alpha;
};

#endif //STRESSBC_H
