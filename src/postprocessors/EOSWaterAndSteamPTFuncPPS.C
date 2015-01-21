#include "EOSWaterAndSteamPTFuncPPS.h"
#include "Function.h"

template <>
InputParameters validParams<EOSWaterAndSteamPTFuncPPS>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addParam<UserObjectName>("water_steam_properties", "EOS functions, calculates water steam properties");
    params.addParam<Real>("pressure_value", "constant pressure value used to calculate density for testing");
    params.addRequiredParam<FunctionName>("temperature_value", "incremented temperature value used to calculate density for testing");
    params.addParam<bool>("water_phase", true, "Flag to run test in liquid water phase");
    params.addParam<bool>("test_density", true, "Flag true to run test for density output, false to run test for enthalpy output");
  return params;
}

EOSWaterAndSteamPTFuncPPS::EOSWaterAndSteamPTFuncPPS(const std::string & name, InputParameters params) :
    GeneralPostprocessor(name, params),
    _water_steam_properties(getUserObject<WaterSteamEOS>("water_steam_properties")),
    _pressure(getParam<Real>("pressure_value")),
    _temperature(getFunction("temperature_value")),
    _water_phase(getParam<bool>("water_phase")),
    _test_density(getParam<bool>("test_density"))

{}

EOSWaterAndSteamPTFuncPPS::~EOSWaterAndSteamPTFuncPPS()
{
}

void
EOSWaterAndSteamPTFuncPPS::initialize()
{
}

void
EOSWaterAndSteamPTFuncPPS::execute()
{
}

PostprocessorValue
EOSWaterAndSteamPTFuncPPS::getValue()
{
    //this postprocessor is used to test the waterEquationOfStatePT function in the WaterSteamEOS UserObject
    //incremented values of pressre and temperature are inputed and either density (water/steam) or enthalpy (water/steam) is outputted
    //within the EOSWaterAndSteamPTFunc_test, the output of this postprocessor is compared to a csv file containing IAWPS97 data for verification

    //first loop is used to test density water output
    if (_test_density == true)
    {
        if (_water_phase == true)
        {
            Real _dummy_var;
            Real _density_water;

            _water_steam_properties.waterEquationOfStatePT (_pressure, _temperature.value(_t,Point(0,0,0)), _dummy_var, _density_water);

            return _density_water;
        }
        //second loop to test density steam output
        else
        {
            Real _dummy_var;
            Real _density_steam;

            _water_steam_properties.steamEquationOfStatePT (_pressure, _temperature.value(_t,Point(0,0,0)), _dummy_var, _density_steam);

            return _density_steam;
        }
    }
    //third loop to test enthalpy water output
    else
    {
        if (_water_phase == true)
        {
            Real _dummy_var;
            Real _enthalpy_water;

            _water_steam_properties.waterEquationOfStatePT (_pressure, _temperature.value(_t,Point(0,0,0)), _enthalpy_water, _dummy_var);

            return (_enthalpy_water / 1000);
        }
        //fourth loop to test enthalpy steam output
        else
        {
            Real _dummy_var;
            Real _enthalpy_steam;

            _water_steam_properties.steamEquationOfStatePT (_pressure, _temperature.value(_t,Point(0,0,0)), _enthalpy_steam, _dummy_var);

            return (_enthalpy_steam / 1000);
        }
    }
}
