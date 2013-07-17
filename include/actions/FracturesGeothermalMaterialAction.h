#ifndef FRACTURESGEOTHERMALMATERIALACTION_H
#define FRACTURESGEOTHERMALMATERIALACTION_H

#include "Action.h"
#include "MooseTypes.h"

class FracturesGeothermalMaterialAction;

template<>
InputParameters validParams<FracturesGeothermalMaterialAction>();

class FracturesGeothermalMaterialAction : public Action
{
public:
  FracturesGeothermalMaterialAction(const std::string & name, InputParameters params);

  virtual void act();
    
  virtual void addSolidMechanicsMaterial(InputParameters shared_params);
  virtual void addHeatTransportMaterial(InputParameters shared_params);
  virtual void addFluidFlowMaterial(InputParameters shared_params);
  virtual void addChemicalReactionsMaterial(InputParameters shared_params);

private:
  bool _has_heat_tran;
  bool _has_fluid_flow;
  bool _has_solid_mech;
  bool _has_chem_react;
};


#endif
