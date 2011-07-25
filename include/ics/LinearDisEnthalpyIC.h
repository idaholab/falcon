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

