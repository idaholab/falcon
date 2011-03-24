#include "ImplicitEuler.h"

#ifndef HUYAKORNMASSIMPLICITEULER
#define HUYAKORNMASSIMPLICITEULER

//Forward Declarations
class HuyakornMassImplicitEuler;

template<>
InputParameters validParams<HuyakornMassImplicitEuler>();

class HuyakornMassImplicitEuler : public ImplicitEuler
{
public:

  HuyakornMassImplicitEuler(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();


   MaterialProperty<Real> & _density;
   MaterialProperty<Real> & _density_old;
   MaterialProperty<Real> & _porosity;

};
#endif //HUYAKORNMASSIMPLICITEULER
