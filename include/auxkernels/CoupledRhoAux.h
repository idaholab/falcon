#ifndef COUPLEDRHOAUX_H
#define COUPLEDRHOAUX_H

#include "AuxKernel.h"


//Forward Declarations
class CoupledRhoAux;

template<>
InputParameters validParams<CoupledRhoAux>();

/** 
 * Coupled auxiliary value
 */
class CoupledRhoAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CoupledRhoAux(const std::string & name, InputParameters parameters);

  virtual ~CoupledRhoAux() {}
  
protected:
  virtual Real computeValue();

    
  MaterialProperty<Real> & _rho_w;
  MaterialProperty<Real> & _rho_s;
  MaterialProperty<Real> & _sat_w;
  MaterialProperty<Real> & _sat_s;
};

#endif //COUPLEDRHOAUX_H
