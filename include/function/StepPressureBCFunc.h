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

#ifndef StepPressureBCFunc_H
#define StepPressureBCFunc_H

#include "Function.h"

class StepPressureBCFunc;

template<>
InputParameters validParams<StepPressureBCFunc>();

class StepPressureBCFunc : public Function
{
public:
StepPressureBCFunc(const std::string & name, InputParameters parameters);

virtual Real value(Real t, const Point & p);

protected:
Real _p1;
Real _p2;
Real _p3;
Real _p4;
Real _t1;
Real _t2;
Real _t3;
};

#endif //StepPressureBCFunc
