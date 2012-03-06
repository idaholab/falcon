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

#ifndef LINEARDISENTHALPYIC_H
#define LINEARDISENTHALPYIC_H

// MOOSE includes
#include "Kernel.h"
#include "InitialCondition.h"

// LibMesh includes
#include <parameters.h>
#include <point.h>
#include <vector_value.h>

// system includes
#include <string>

//Forward Declarations
class LinearDisEnthalpyIC;

template<>
InputParameters validParams<LinearDisEnthalpyIC> ();

class LinearDisEnthalpyIC: public InitialCondition
{

public:
  LinearDisEnthalpyIC(const std::string & name, InputParameters parameters);

  virtual Real value(const Point & p);

private:
  Real _basevalue;
  Real _slope;

};
#endif //LINEARDISENTHALPYIC_H

