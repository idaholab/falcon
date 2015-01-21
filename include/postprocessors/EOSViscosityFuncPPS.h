#ifndef EOSVISCOSITYFUNCPPS_H
#define EOSVISCOSITYFUNCPPS_H

#include "GeneralPostprocessor.h"
#include "WaterSteamEOS.h"

class EOSViscosityFuncPPS;

template <>
InputParameters validParams<EOSViscosityFuncPPS>();

class EOSViscosityFuncPPS : public GeneralPostprocessor
{
public:
  EOSViscosityFuncPPS (const std::string & name, InputParameters params);
  virtual ~EOSViscosityFuncPPS();

  virtual void initialize();
  virtual void execute();
  virtual PostprocessorValue getValue();

protected:
  const WaterSteamEOS & _water_steam_properties;

    Real _density;
    Function & _temperature;

    bool _water_phase;
};

#endif /* EOSVISCOSITYFUNCPPS_H */
