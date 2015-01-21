#ifndef EOSPHASEDETERMINEFUNCPPS_H
#define EOSPHASEDETERMINEFUNCPPS_H

#include "GeneralPostprocessor.h"
#include "WaterSteamEOS.h"

class EOSPhaseDetermineFuncPPS;

template <>
InputParameters validParams<EOSPhaseDetermineFuncPPS>();

class EOSPhaseDetermineFuncPPS : public GeneralPostprocessor
{
public:
  EOSPhaseDetermineFuncPPS(const std::string & name, InputParameters params);
  virtual ~EOSPhaseDetermineFuncPPS();

  virtual void initialize();
  virtual void execute();
  virtual PostprocessorValue getValue();

protected:
  const WaterSteamEOS & _water_steam_properties;

    Real _enthalpy;
    Function & _pressure;

    bool _test_phase;
    bool _test_saturation_temp;
    bool _test_saturation_enthalpy_water;
    bool _test_saturation_enthalpy_steam;
    bool _test_saturation_density_water;
    bool _test_saturation_density_steam;

};

#endif /* EOSPHASEDETERMINEFUNCPPS_H */
