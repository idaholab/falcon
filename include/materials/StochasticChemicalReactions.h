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

#ifndef STOCHASTICCHEMICALREACTIONS_H
#define STOCHASTICCHEMICALREACTIONS_H

#include "StochasticPorousMedia.h"


//Forward Declarations
class StochasticChemicalReactions;

template<>
InputParameters validParams<StochasticChemicalReactions>();

/**
 * Simple material with StochasticPorousMedia properties.
 */
class StochasticChemicalReactions : virtual public StochasticPorousMedia
{
public:
  StochasticChemicalReactions(const std::string & name,
              InputParameters parameters);
  
protected:
//  virtual void initQpStatefulProperties();
  virtual void computeProperties();
////Grab user input parameters
  Real _input_chem_diff;
  std::vector<Real> _mineral;
  std::vector<Real> _molecular_weight;
  std::vector<Real> _mineral_density;
  std::vector<VariableValue *> _vals;
    
  bool _has_permeability;
  VariableValue & _input_permeability;
  Real _input_porosity;
    
////Declare material properties
  MaterialProperty<Real> & _diffusivity;
  
};


#endif //STOCHASTICCHEMICALREACTIONS_H
