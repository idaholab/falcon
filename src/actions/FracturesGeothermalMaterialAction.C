#include "FracturesGeothermalMaterialAction.h"

#include "Factory.h"
#include "FEProblem.h"
#include "Parser.h"

template<>
InputParameters validParams<FracturesGeothermalMaterialAction>()
{
  InputParameters params = validParams<Action>();
  params.addParam<std::vector<SubdomainName> >("block", "The list of ids of the blocks (subdomain) that these kernels will be applied to");
//H, T, M, C, TH, THM, TM, HM, THMC ...
  params.addRequiredParam<bool>("solid_mechanics", "Solid mechancis material");
  params.addRequiredParam<bool>("heat_transport", "Heat transport material");
  params.addRequiredParam<bool>("fluid_flow", "Fluid flow material");
  params.addRequiredParam<bool>("chemical_reactions", "Chemcial reactions material");
//Main non-linear coupled variables
  params.addParam<NonlinearVariableName>("pressure", "Coupled pressure variable, [Pa]");
  params.addParam<NonlinearVariableName>("temperature", "Coupled temperature variable, [K]");
  params.addParam<NonlinearVariableName>("enthalpy", "Coupled enthalpy variable, [J]");
  params.addParam<NonlinearVariableName>("x_disp", "Coupled x_displacement variable, [m]");
  params.addParam<NonlinearVariableName>("y_disp", "Coupled y_displacement variable, [m]");
  params.addParam<NonlinearVariableName>("z_disp", "Coupled z_displacement variable, [m]");
  params.addParam<std::vector<NonlinearVariableName> >("v", "The list of primary species to add");
//Input parameters
  //general - porous_media
  params.addParam<Real>("gravity", 9.80665, "[m/s^2]");
  params.addParam<Real>("gx", 0.0, "dimentionless");
  params.addParam<Real>("gy", 0.0, "dimentionless");
  params.addParam<Real>("gz", 1.0, "dimentionless");
  //general - solid_mechanics
  params.addParam<Real>("poissons_ratio", 0.2, "dimentionless");
  params.addParam<Real>("biot_coeff", 1.0, "dimentionless");
  params.addParam<Real>("biot_modulus", 2.5e10, "dimenstionless");
  //general - heat_transport
  params.addParam<Real>("specific_heat_rock", 0.92e3, "[J/kg.K]");
  params.addParam<Real>("specific_heat_water", 4.186e3, "[J/kg.K]");
  //general - fluid_flow
  params.addParam<Real>("constant_density", 1000, "[kg/m^3]");
  params.addParam<Real>("constant_viscosity", 0.12e-3, "[Pa.s]");
  params.addParam<bool>("temp_dependent_fluid_props", true, "flag true if single-phase and fluid properties are temperature dependent");
  params.addParam<UserObjectName>("water_steam_properties", "If temp_dependent_fluid_props = true, select which user object to use for EOS calculations");

  //matrix - porous_media
  params.addParam<Real>("matrix_permeability", 1e-12, "[m^2]");
  params.addParam<Real>("matrix_porosity", 0.3,"dimentionless");
  params.addParam<Real>("matrix_density", 2.5e3, "[kg/m^3]");
  //matrix - solid_mechancis
  params.addParam<Real>("matrix_thermal_expansion",1.0e-6,"thermal expansion coefficient of matrix, [1/K]");
  params.addParam<Real>("matrix_youngs_modulus",1.50e10,"youngs modulus of matrix, [Pa]");
  params.addParam<Real>("matrix_thermal_strain_ref_temp",293.15,"Initial reference temperature of matrix where there is no thermal strain, [K]");
  //matrix - heat_transport
  params.addParam<Real>("matrix_thermal_conductivity",2.5, "thermal conductivity of matrix rock, [W/(m.K)]");
  //matrix - chemcial_reactions
  params.addParam<Real>("matrix_diffusivity", 1e-8, "[kg/m^3]");
  params.addParam<std::vector<Real> >("matrix_mineral", std::vector<Real>(1, 16.65), "[mol/L] solution");
  params.addParam<std::vector<Real> >("matrix_mineral_density", std::vector<Real>(1, 100.08), "[g/cm^3]");
  params.addParam<std::vector<Real> >("matrix_molecular_weight", std::vector<Real>(1, 2.5), "[g/mol]");

  //fractures - general
  params.addParam<std::vector<AuxVariableName> >("fractures", "coupled aux variable that maps where the fracture are");
  params.addParam<Real>("fracture_num", 0, "number in fracture map that indicates fractures");
  params.addParam<Real>("model_fracture_aperture", 1.0, "width of fracture/high permeability area in the model");
  params.addParam<bool>("has_strain_dependent_permeability", false, "switch for displacement dependent permeability changes");
  //fractures - porous_media
  params.addParam<Real>("fracture_permeability", 1e-12, "intrinsic permeability of fractures (m^2)");
  params.addParam<Real>("fracture_porosity", 0.3, "rock porosity of fractures");
  params.addParam<Real>("fracture_density", 2500, "rock density of fractures (kg/m^3)");
  //fractures - solid_mechanics
  params.addParam<Real>("fracture_thermal_expansion",1.0e-6,"thermal expansion coefficient of fractures, [1/K]");
  params.addParam<Real>("fracture_youngs_modulus",1.50e10,"youngs modulus of fractures, [Pa]");
  params.addParam<Real>("fracture_thermal_strain_ref_temp",293.15,"Initial reference temperature of fractures where there is no thermal strain, [K]");
  //fractures - heat_transport
  params.addParam<Real>("fracture_thermal_conductivity", 2.5, "thermal conductivity of fractures, [W/(m.K)]");
  //fractures - chemical_reactions
  params.addParam<Real>("fracture_diffusivity", 1.0e-8, "the chemical diffusivity of the fractures, [m^2/s]");
  params.addParam<std::vector<Real> >("fracture_mineral", std::vector<Real>(1, 16.65), "Initial mineral concentration in fractures, [mol/L] solution");
  params.addParam<std::vector<Real> >("fracture_molecular_weight", std::vector<Real>(1, 100.08), "The molecular weight of mineral in fractures, [g/mol]");
  params.addParam<std::vector<Real> >("fracture_mineral_density", std::vector<Real>(1, 2.5), "The density of mineral in fractures, [g/cm^3]");

  return params;
}

FracturesGeothermalMaterialAction::FracturesGeothermalMaterialAction(const std::string & name, InputParameters params) :
  Action(name, params),
//H, T, M, C, TH, THM, TM, HM, THMC ...
    _has_heat_tran(getParam<bool>("heat_transport")),       //T
    _has_fluid_flow(getParam<bool>("fluid_flow")),          //H
    _has_solid_mech(getParam<bool>("solid_mechanics")),     //M
    _has_chem_react(getParam<bool>("chemical_reactions"))   //C
{
}

void
FracturesGeothermalMaterialAction::act()
{
    // input parameters for this action are split up into 5 sets of parameters
    // shared_params = all paramerters of the base class and all valid NL variables,
    //                 this gets added to each of the following 4 parameter sets
    // sm_params = all parameters needed for FracturesSolidMechanics material
    // ht_params = all parameters needed for FracturesHeatTransport material
    // ff_params = all parameters needed for FracturesFluidFlow material
    // cr_params = all parameters needed for FracturesChemicalReactions material

    //get input parameters from base class FracturesPorousMedia
    InputParameters shared_params = _factory.getValidParams("PorousMedia");
    //get block #/name that we want to assign this material action to and add it to the shared_params
    std::vector<SubdomainName> block = getParam<std::vector<SubdomainName> >("block");
    shared_params.set<std::vector<SubdomainName> >("block") = block;

    //check which NL variables are coupled and add them to shared_params if valid
    if (_pars.isParamValid("x_disp"))
    {
        std::vector<NonlinearVariableName> x_var (1, getParam<NonlinearVariableName>("x_disp"));
        shared_params.set<std::vector<NonlinearVariableName> >("x_disp") = x_var;
    }
    if (_pars.isParamValid("y_disp"))
    {
        std::vector<NonlinearVariableName> y_var (1, getParam<NonlinearVariableName>("y_disp"));
        shared_params.set<std::vector<NonlinearVariableName> >("y_disp") = y_var;
    }
    if (_pars.isParamValid("z_disp"))
    {
        std::vector<NonlinearVariableName> z_var (1, getParam<NonlinearVariableName>("z_disp"));
        shared_params.set<std::vector<NonlinearVariableName> >("z_disp") = z_var;
    }
    if (_pars.isParamValid("pressure"))
    {
        std::vector<NonlinearVariableName> press_var (1, getParam<NonlinearVariableName>("pressure"));
        shared_params.set<std::vector<NonlinearVariableName> >("pressure") = press_var;
    }
    if (_pars.isParamValid("enthalpy"))
    {
        std::vector<NonlinearVariableName> enth_var (1, getParam<NonlinearVariableName>("enthalpy"));
        shared_params.set<std::vector<NonlinearVariableName> >("enthalpy") = enth_var;
    }
    if (_pars.isParamValid("temperature"))
    {
        std::vector<NonlinearVariableName> temp_var (1, getParam<NonlinearVariableName>("temperature"));
        shared_params.set<std::vector<NonlinearVariableName> >("temperature") = temp_var;
    }
    if (_pars.isParamValid("v"))
    {
        std::vector<NonlinearVariableName> chem_vars = getParam<std::vector<NonlinearVariableName> >("v");
        shared_params.set<std::vector<NonlinearVariableName> >("v") = chem_vars;
    }

    //get base class (FracturesPorousMedia) paramerters from input
    Real gravity = getParam<Real>("gravity");
    Real gx = getParam<Real>("gx");
    Real gy = getParam<Real>("gy");
    Real gz = getParam<Real>("gz");

    Real matrix_permeability = getParam<Real>("matrix_permeability");
    Real matrix_porosity = getParam<Real>("matrix_porosity");
    Real matrix_density = getParam<Real>("matrix_density");

    std::vector<AuxVariableName> fractures = getParam<std::vector<AuxVariableName> >("fractures");
    Real fracture_num = getParam<Real>("fracture_num");
    Real model_fracture_aperture = getParam<Real>("model_fracture_aperture");
    bool has_strain_dependent_permeability = getParam<bool>("has_strain_dependent_permeability");

    Real fracture_permeability = getParam<Real>("fracture_permeability");
    Real fracture_porosity = getParam<Real>("fracture_porosity");
    Real fracture_density = getParam<Real>("fracture_density");

    //add these base class paramerters to shared_params, since all dependent classes need these parameters
    shared_params.set<Real>("gravity") = gravity;
    shared_params.set<Real>("gx") = gx;
    shared_params.set<Real>("gy") = gy;
    shared_params.set<Real>("gz") = gz;

    shared_params.set<Real>("matrix_permeability") = matrix_permeability;
    shared_params.set<Real>("matrix_porosity") = matrix_porosity;
    shared_params.set<Real>("matrix_density") = matrix_density;

    shared_params.set<std::vector<AuxVariableName> >("fractures") = fractures;
    shared_params.set<Real>("fracture_num") = fracture_num;
    shared_params.set<Real>("model_fracture_aperture") = model_fracture_aperture;
    shared_params.set<bool>("has_strain_dependent_permeability") = has_strain_dependent_permeability;

    shared_params.set<Real>("fracture_permeability") = fracture_permeability;
    shared_params.set<Real>("fracture_porosity") = fracture_porosity;
    shared_params.set<Real>("fracture_density") = fracture_density;

    shared_params.set<bool>("has_chem_reactions") = _has_chem_react;

    // based upon user input for this action, we will appropriately add materials (and their respective parameters)
    // for a customizable mix-and-match THMC material

    // hydro problems: _has_fluid_flow = true
    // thermo problems: _has_heat_tran = true
    // mechanical problems: _has_solid_mech = true
    // chemical problems: _has_chem_react = true

    if (_has_fluid_flow)
        addFluidFlowMaterial(shared_params);

    if (_has_heat_tran)
        addHeatTransportMaterial(shared_params);

    if (_has_solid_mech)
        addSolidMechanicsMaterial(shared_params);

    if (_has_chem_react)
        addChemicalReactionsMaterial(shared_params);
}

void
FracturesGeothermalMaterialAction::addSolidMechanicsMaterial(InputParameters shared_params)
{
    //get input parameters from FracturesSolidMechanics material
    InputParameters sm_params = _factory.getValidParams("FracturesSolidMechanics");

    //add shared_params from above
    sm_params += shared_params;

    //get FracturesSolidMechancis paramerters from input file
    Real poissons_ratio = getParam<Real>("poissons_ratio");
    Real biot_coeff = getParam<Real>("biot_coeff");
    Real biot_modulus = getParam<Real>("biot_modulus");

    Real matrix_thermal_expansion = getParam<Real>("matrix_thermal_expansion");
    Real matrix_youngs_modulus = getParam<Real>("matrix_youngs_modulus");
    Real matrix_thermal_strain_ref_temp = getParam<Real>("matrix_thermal_strain_ref_temp");

    Real fracture_thermal_expansion = getParam<Real>("fracture_thermal_expansion");
    Real fracture_youngs_modulus = getParam<Real>("fracture_youngs_modulus");
    Real fracture_thermal_strain_ref_temp = getParam<Real>("fracture_thermal_strain_ref_temp");

    //add these paramerters to sm_params
    sm_params.set<Real>("poissons_ratio") = poissons_ratio;
    sm_params.set<Real>("biot_coeff") = biot_coeff;
    sm_params.set<Real>("biot_modulus") = biot_modulus;

    sm_params.set<Real>("matrix_thermal_expansion") = matrix_thermal_expansion;
    sm_params.set<Real>("matrix_youngs_modulus") = matrix_youngs_modulus;
    sm_params.set<Real>("matrix_thermal_strain_ref_temp") = matrix_thermal_strain_ref_temp;

    sm_params.set<Real>("fracture_thermal_expansion") = fracture_thermal_expansion;
    sm_params.set<Real>("fracture_youngs_modulus") = fracture_youngs_modulus;
    sm_params.set<Real>("fracture_thermal_strain_ref_temp") = fracture_thermal_strain_ref_temp;

    //add FracturesSolidMechanics material using sm_params
    _problem->addMaterial("FracturesSolidMechanics", "fractures_solid_mechanics", sm_params);
}

void
FracturesGeothermalMaterialAction::addHeatTransportMaterial(InputParameters shared_params)
{
    //get input parameters from FracturesHeatTransport material
    InputParameters ht_params = _factory.getValidParams("FracturesHeatTransport");

    //add shared_params from above
    ht_params += shared_params;

    //get FracturesHeatTransport paramerters from input file
    Real specific_heat_rock = getParam<Real>("specific_heat_rock");
    Real specific_heat_water = getParam<Real>("specific_heat_water");

    Real matrix_thermal_conductivity = getParam<Real>("matrix_thermal_conductivity");
    Real fracture_thermal_conductivity = getParam<Real>("fracture_thermal_conductivity");

    //add these paramerters to ht_params
    ht_params.set<Real>("specific_heat_rock") = specific_heat_rock;
    ht_params.set<Real>("specific_heat_water") = specific_heat_water;

    ht_params.set<Real>("matrix_thermal_conductivity") = matrix_thermal_conductivity;
    ht_params.set<Real>("fracture_thermal_conductivity") = fracture_thermal_conductivity;

    //add FracturesHeatTransport material using ht_params
    _problem->addMaterial("FracturesHeatTransport", "fractures_heat_transport", ht_params);
}

void
FracturesGeothermalMaterialAction::addFluidFlowMaterial(InputParameters shared_params)
{
    //get input parameters from FracturesFliudFlow material
    InputParameters ff_params = _factory.getValidParams("FracturesFluidFlow");

    //add shared_params from above
    ff_params += shared_params;

    //get FracturesFluidFlow paramerters from input file and add these paramerters to ff_params
    bool temp_dependent_fluid_props = getParam<bool>("temp_dependent_fluid_props");
    ff_params.set<bool>("temp_dependent_fluid_props") = temp_dependent_fluid_props;

    //if temp_dependent_fluid_props = true, then we need to also grab the UserObject that
    //calculates our water/steam EOS
    if (temp_dependent_fluid_props == true)
    {
        UserObjectName water_steam_properties = getParam<UserObjectName>("water_steam_properties");
        ff_params.set<UserObjectName>("water_steam_properties") = water_steam_properties;
    }
    //else we just grab the constant fluid density and viscosity values
    else
    {
        Real constant_density = getParam<Real>("constant_density");
        Real constant_viscosity = getParam<Real>("constant_viscosity");
        ff_params.set<Real>("constant_density") = constant_density;
        ff_params.set<Real>("constant_viscosity") = constant_viscosity;
    }

    //add FracturesFluidFlow material using ff_params
    _problem->addMaterial("FracturesFluidFlow", "fractures_fluid_flow", ff_params);
}

void
FracturesGeothermalMaterialAction::addChemicalReactionsMaterial(InputParameters shared_params)
{
    //get input parameters from FracturesChemicalReactions material
    InputParameters cr_params = _factory.getValidParams("FracturesChemicalReactions");

    //add shared_params from above
    cr_params += shared_params;

    //get FracturesChemicalReactions paramerters from input file
    Real matrix_diffusivity = getParam<Real>("matrix_diffusivity");
    std::vector<Real> matrix_mineral = getParam<std::vector<Real> >("matrix_mineral");
    std::vector<Real> matrix_molecular_weight = getParam<std::vector<Real> >("matrix_molecular_weight");
    std::vector<Real> matrix_mineral_density = getParam<std::vector<Real> >("matrix_mineral_density");

    Real fracture_diffusivity = getParam<Real>("fracture_diffusivity");
    std::vector<Real> fracture_mineral = getParam<std::vector<Real> >("fracture_mineral");
    std::vector<Real> fracture_molecular_weight = getParam<std::vector<Real> >("fracture_molecular_weight");
    std::vector<Real> fracture_mineral_density = getParam<std::vector<Real> >("fracture_mineral_density");

    //add these paramerters to cr_params
    cr_params.set<Real>("matrix_diffusivity") = matrix_diffusivity;
    cr_params.set<std::vector<Real> >("matrix_mineral") = matrix_mineral;
    cr_params.set<std::vector<Real> >("matrix_molecular_weight") = matrix_molecular_weight;
    cr_params.set<std::vector<Real> >("matrix_mineral_density") = matrix_mineral_density;

    cr_params.set<Real>("fracture_diffusivity") = fracture_diffusivity;
    cr_params.set<std::vector<Real> >("fracture_mineral") = fracture_mineral;
    cr_params.set<std::vector<Real> >("fracture_molecular_weight") = fracture_molecular_weight;
    cr_params.set<std::vector<Real> >("fracture_mineral_density") = fracture_mineral_density;

    //add FracturesChemicalReactions material using cr_params
    _problem->addMaterial("FracturesChemicalReactions", "fractures_chemical_reactions", cr_params);
}
