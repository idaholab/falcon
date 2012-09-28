#include "WaterSteamEOSDensityPPS_PT.h"
#include "Function.h"

template <>
InputParameters validParams<WaterSteamEOSDensityPPS_PT>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addParam<UserObjectName>("water_steam_properties", "EOS functions, calculates water steam properties");
    params.addParam<Real>("pressure_value", "bleh ... ");
    params.addRequiredParam<FunctionName>("temperature_value", "bleh ... ");
  return params;
}

WaterSteamEOSDensityPPS_PT::WaterSteamEOSDensityPPS_PT(const std::string & name, InputParameters params) :
    GeneralPostprocessor(name, params),
    _water_steam_properties(getUserObject<WaterSteamEOS>("water_steam_properties")),
    _pressure(getParam<Real>("pressure_value")),
    _temperature(getFunction("temperature_value"))

{}

WaterSteamEOSDensityPPS_PT::~WaterSteamEOSDensityPPS_PT()
{
}

void
WaterSteamEOSDensityPPS_PT::initialize()
{
}

void
WaterSteamEOSDensityPPS_PT::execute()
{
}

PostprocessorValue
WaterSteamEOSDensityPPS_PT::getValue()
{
    Real _dummy_var;
    Real _density_water;
    
    _water_steam_properties.waterEquationOfStatePT (_pressure, _temperature.value(_t,Point(0,0,0)), _dummy_var, _density_water);
    
    return _density_water;
}
