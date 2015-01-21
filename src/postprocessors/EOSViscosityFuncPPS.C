#include "EOSViscosityFuncPPS.h"
#include "Function.h"

template <>
InputParameters validParams<EOSViscosityFuncPPS>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addParam<UserObjectName>("water_steam_properties", "EOS functions, calculates water steam properties");
    params.addParam<Real>("density_value", "constant pressure value used to calculate density for testing");
    params.addRequiredParam<FunctionName>("temperature_value", "incremented temperature value used to calculate density for testing");
    params.addParam<bool>("water_phase", true, "Flag to run test in liquid water phase");
  return params;
}

EOSViscosityFuncPPS::EOSViscosityFuncPPS(const std::string & name, InputParameters params) :
    GeneralPostprocessor(name, params),
    _water_steam_properties(getUserObject<WaterSteamEOS>("water_steam_properties")),
    _density(getParam<Real>("density_value")),
    _temperature(getFunction("temperature_value")),
    _water_phase(getParam<bool>("water_phase"))

{}

EOSViscosityFuncPPS::~EOSViscosityFuncPPS()
{
}

void
EOSViscosityFuncPPS::initialize()
{
}

void
EOSViscosityFuncPPS::execute()
{
}

PostprocessorValue
EOSViscosityFuncPPS::getValue()
{
    //this postprocessor is used to test the viscosity function in the WaterSteamEOS UserObject
    //incremented values of density and temperature are inputed and viscosity (water/steam) is outputted.
    //saturated mixture viscosity testing will follow soon.
    //within the EOSViscosityFunc_test, the output of this postprocessor is compared to a csv file containing IAWPS97 data for verification


    if (_water_phase == true)
    {
        Real _viscosity_water;

        _water_steam_properties.viscosity (_density, _temperature.value(_t,Point(0,0,0)), _viscosity_water);

        return _viscosity_water;
    }
    else
    {
        Real _viscosity_steam;

        _water_steam_properties.viscosity (_density, _temperature.value(_t,Point(0,0,0)), _viscosity_steam);

        return _viscosity_steam;
    }
}
