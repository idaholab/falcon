#ifndef WATERSTEAMEOSDENSITYPPS_PT_H
#define WATERSTEAMEOSDENSITYPPS_PT_H

#include "GeneralPostprocessor.h"
#include "WaterSteamEOS.h"

class WaterSteamEOSDensityPPS_PT;

template <>
InputParameters validParams<WaterSteamEOSDensityPPS_PT>();

class WaterSteamEOSDensityPPS_PT : public GeneralPostprocessor
{
public:
  WaterSteamEOSDensityPPS_PT(const std::string & name, InputParameters params);
  virtual ~WaterSteamEOSDensityPPS_PT();

  virtual void initialize();
  virtual void execute();
  virtual PostprocessorValue getValue();

protected:
  const WaterSteamEOS & _water_steam_properties;
    
    Real _pressure;
    Function & _temperature;
};

#endif /* WATERSTEAMEOSDENSITYPPS_PT_H */
