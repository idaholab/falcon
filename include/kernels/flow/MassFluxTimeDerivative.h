#include "TimeDerivative.h"

#ifndef MASSFLUXTIMEDERIVATIVE
#define MASSFLUXTIMEDERIVATIVE

//Forward Declarations
class MassFluxTimeDerivative;

template<>
InputParameters validParams<MassFluxTimeDerivative>();

class MassFluxTimeDerivative : public TimeDerivative
{
public:

  MassFluxTimeDerivative(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  MaterialProperty<Real> & _density_water;
  MaterialProperty<Real> & _compressibility;
  MaterialProperty<Real> & _porosity;

};
#endif //MASSFLUXTIMEDERIVATIVE
