#include "EOSPhaseDetermineFuncPPS.h"
#include "Function.h"

template <>
InputParameters validParams<EOSPhaseDetermineFuncPPS>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addParam<UserObjectName>("water_steam_properties", "EOS functions, calculates water steam properties");
    params.addParam<Real>("enthalpy_value", "constant enthalpy value used for testing");
    params.addRequiredParam<FunctionName>("pressure_value", "incremented pressure value used for testing");
    params.addParam<bool>("test_phase", false, "Flag to run test to determine phase of water (liq., steam, sat. mix.)");
    params.addParam<bool>("test_saturation_temp", false, "Flag to run test to determine saturation temperature");
    params.addParam<bool>("test_saturation_enthalpy_water", false, "Flag to run test to determine saturated water enthalpy");
    params.addParam<bool>("test_saturation_enthalpy_steam", false, "Flag to run test to determine saturated steam enthalpy");
    params.addParam<bool>("test_saturation_density_water", false, "Flag to run test to determine saturated water density");
    params.addParam<bool>("test_saturation_density_steam", false, "Flag to run test to determine saturated steam density");
    return params;
}

EOSPhaseDetermineFuncPPS::EOSPhaseDetermineFuncPPS(const std::string & name, InputParameters params) :
    GeneralPostprocessor(name, params),
    _water_steam_properties(getUserObject<WaterSteamEOS>("water_steam_properties")),
    _enthalpy(getParam<Real>("enthalpy_value")),
    _pressure(getFunction("pressure_value")),
    _test_phase(getParam<bool>("test_phase")),
    _test_saturation_temp(getParam<bool>("test_saturation_temp")),
    _test_saturation_enthalpy_water(getParam<bool>("test_saturation_enthalpy_water")),
    _test_saturation_enthalpy_steam(getParam<bool>("test_saturation_enthalpy_steam")),
    _test_saturation_density_water(getParam<bool>("test_saturation_density_water")),
    _test_saturation_density_steam(getParam<bool>("test_saturation_density_steam"))

{}

EOSPhaseDetermineFuncPPS::~EOSPhaseDetermineFuncPPS()
{
}

void
EOSPhaseDetermineFuncPPS::initialize()
{
}

void
EOSPhaseDetermineFuncPPS::execute()
{
}

PostprocessorValue
EOSPhaseDetermineFuncPPS::getValue()
{

    //this postprocessor is used to test the phaseDetermine function in the WaterSteamEOS UserObject
    //incremented values of pressre and enthalpy are inputed and either the phase, saturation temperature, saturation enthalpy (water/steam),
    //or saturation enthalpy (water/steam) is outputted depending on what boolean is flag as 'true' (see above).  
    //Currently only the phase output is being tested, the rest is to follow.
    //Within the EOSPhaseDetermineFunc_test, the output of this postprocessor is compared to a csv file containing IAWPS97 data for verification

    Real _phase, _temp_sat;
    Real _enth_water_sat, _enth_steam_sat;
    Real _dens_water_sat, _dens_steam_sat;
    
    _water_steam_properties.phaseDetermine (_enthalpy, _pressure.value(_t,Point(0,0,0)), _phase, _temp_sat, _enth_water_sat, _enth_steam_sat, _dens_water_sat, _dens_steam_sat);
    
    if (_test_phase == true)
    {   
        return _phase;
    }
    else if(_test_saturation_temp == true)
    {
        return _temp_sat;
    }
    else if (_test_saturation_enthalpy_water == true)
    {
        return _enth_water_sat;
    }
    else if (_test_saturation_enthalpy_steam == true)
    {
        return _enth_steam_sat;
    }
    else if (_test_saturation_density_water == true)
    {
        return _dens_water_sat;
    }
    else if (_test_saturation_density_steam == true)
    {
        return _dens_steam_sat;
    }
    else
    {
        std::cout << "You have not specified an output perameter to test" << std::endl;
        return 0;
    }
    
}
