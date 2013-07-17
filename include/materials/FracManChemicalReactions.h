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

#ifndef FRACMANCHEMICALREACTIONS_H
#define FRACMANCHEMICALREACTIONS_H

#include "FracManPorousMedia.h"


//Forward Declarations
class FracManChemicalReactions;

template<>
InputParameters validParams<FracManChemicalReactions>();

/**
 * Simple material with PorousMedia properties.
 */
class FracManChemicalReactions : virtual public FracManPorousMedia
{
public:
  FracManChemicalReactions(const std::string & name,
              InputParameters parameters);
  
protected:
//  virtual void initQpStatefulProperties();
  virtual void computeProperties();
////Grab user input parameters
  ////Matrix
  Real _matrix_chem_diff;
  std::vector<Real> _matrix_mineral;
  std::vector<Real> _matrix_molecular_weight;
  std::vector<Real> _matrix_mineral_density;
  ////Fractures
  std::vector<int> _fracture_number_vec;
  std::vector<Real> _fracture_chem_diff_vec;
  std::vector<Real> _fracture_mineral_vec;
  std::vector<Real> _fracture_molecular_weight_vec;
  std::vector<Real> _fracture_mineral_density_vec;
    
////Variables
  std::vector<VariableValue *> _vals;
    
////Storing the number of vector entries into respective local variables
  unsigned int num_frac_vec_entries;
  unsigned int num_chem_diff_vec_entries;
  unsigned int num_mineral_vec_entries;
  unsigned int num_mol_weight_vec_entries;
  unsigned int num_min_dens_vec_entries;
  unsigned int num_vals;
    
////Declare material properties
  MaterialProperty<Real> & _diffusivity;
  
};


#endif //POROUSMEDIA_H
