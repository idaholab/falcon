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
  \file    include/bcs/PressureBC.h
  \author  Yidong Xia 
  \date    October 2014
  \brief   Specify external pressure
 */
//***************************************************************

#ifndef PRESSUREBC_H
#define PRESSUREBC_H

#include "IntegratedBC.h"

//Forward Declarations
class PressureBC;

template<>
InputParameters validParams<PressureBC>();

class PressureBC : public IntegratedBC
{
  public:
  
    PressureBC(const std::string & name, InputParameters parameters);
  
  protected:

    virtual Real computeQpResidual();

   /*! direction
    */
    unsigned int _component;

   /*! Pressure
    */
    Real _pressure;

   /*! Biot effective stress coefficient
    */
    Real _alpha;
};

#endif //PRESSUREBC_H
