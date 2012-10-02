#ifndef EOSWATERANDSTEAMPTFUNCPPS_H
#define EOSWATERANDSTEAMPTFUNCPPS_H

#include "GeneralPostprocessor.h"
#include "WaterSteamEOS.h"

class EOSWaterAndSteamPTFuncPPS;

template <>
InputParameters validParams<EOSWaterAndSteamPTFuncPPS>();

class EOSWaterAndSteamPTFuncPPS : public GeneralPostprocessor
{
public:
  EOSWaterAndSteamPTFuncPPS(const std::string & name, InputParameters params);
  virtual ~EOSWaterAndSteamPTFuncPPS();

  virtual void initialize();
  virtual void execute();
  virtual PostprocessorValue getValue();

protected:
  const WaterSteamEOS & _water_steam_properties;
    
    Real _pressure;
    Function & _temperature;
    
    bool _water_phase;
    bool _test_density;
};

#endif /* EOSWATERANDSTEAMPTFUNCPPS_H */
