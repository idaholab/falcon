#include "Closures1PhaseFalcon.h"
#include "FlowModelSinglePhase.h"
#include "FlowChannel1Phase.h"
#include "HeatTransfer1PhaseBase.h"

registerMooseObject("FalconApp", Closures1PhaseFalcon);

InputParameters
Closures1PhaseFalcon::validParams()
{
  InputParameters params = Closures1PhaseBase::validParams();

  params.addClassDescription("Single-phase closures for tube geometry based on TRACE v5");

  return params;
}

Closures1PhaseFalcon::Closures1PhaseFalcon(const InputParameters & params)
  : Closures1PhaseBase(params)
{
}

void
Closures1PhaseFalcon::checkFlowChannel(const FlowChannelBase & flow_channel) const
{
  if (MooseUtils::absoluteFuzzyEqual(flow_channel.getGravityMagnitude(), 0.0))
    logComponentError(flow_channel.cname(), "Falcon closures assume non-zero gravity.");

  if (flow_channel.getNumberOfHeatTransferConnections() > 0)
  {
    if (!flow_channel.getTemperatureMode())
      logComponentError(flow_channel.cname(),
                        "Heat transfer from heat flux is currently not available. Please, contact "
                        "developers if you have a need for it.");
  }
}

void
Closures1PhaseFalcon::checkHeatTransfer(const HeatTransferBase & /*heat_transfer*/,
                                         const FlowChannelBase & /*flow_channel*/) const
{
}

void
Closures1PhaseFalcon::addMooseObjectsFlowChannel(const FlowChannelBase & flow_channel)
{
  const FlowChannel1Phase & flow_channel_1phase =
      dynamic_cast<const FlowChannel1Phase &>(flow_channel);

  // wall friction material
  if (flow_channel.isParamValid("f"))
    addWallFrictionFunctionMaterial(flow_channel_1phase);
  else
    addWallFrictionChurchillMaterial(flow_channel_1phase);

  const unsigned int n_ht_connections = flow_channel_1phase.getNumberOfHeatTransferConnections();
  if (n_ht_connections > 0)
  {
    if (n_ht_connections > 1)
      addAverageWallTemperatureMaterial(flow_channel_1phase);
    else
      addWallTemperatureFromAuxMaterial(flow_channel_1phase);
  }
}

void
Closures1PhaseFalcon::addMooseObjectsHeatTransfer(const HeatTransferBase & heat_transfer,
                                                   const FlowChannelBase & flow_channel)
{
  const HeatTransfer1PhaseBase & heat_transfer_1phase =
      dynamic_cast<const HeatTransfer1PhaseBase &>(heat_transfer);

  if (heat_transfer.isParamValid("Hw"))
  {
    const FunctionName & Hw_fn_name = heat_transfer.getParam<FunctionName>("Hw");

    const std::string class_name = "ADGenericFunctionMaterial";
    InputParameters params = _factory.getValidParams(class_name);
    params.set<std::vector<SubdomainName>>("block") = flow_channel.getSubdomainNames();
    params.set<std::vector<std::string>>("prop_names") = {
        heat_transfer_1phase.getWallHeatTransferCoefficient1PhaseName()};
    params.set<std::vector<FunctionName>>("prop_values") = {Hw_fn_name};
    _sim.addMaterial(class_name, genName(heat_transfer.name(), "Hw_material"), params);

    heat_transfer.makeFunctionControllableIfConstant(Hw_fn_name, "Hw");
  }
  else
  {
    const FlowChannel1Phase & flow_channel_1phase =
        _sim.getComponentByName<FlowChannel1Phase>(heat_transfer_1phase.getFlowChannelName());
    {
      const std::string class_name = "ADReynoldsNumberMaterial";
      InputParameters params = _factory.getValidParams(class_name);
      params.set<std::vector<SubdomainName>>("block") =
          heat_transfer_1phase.getFlowChannelSubdomains();
      params.set<MaterialPropertyName>("D_h") = FlowModelSinglePhase::HYDRAULIC_DIAMETER;
      params.set<MaterialPropertyName>("rho") = FlowModelSinglePhase::DENSITY;
      params.set<MaterialPropertyName>("vel") = FlowModelSinglePhase::VELOCITY;
      params.set<MaterialPropertyName>("mu") = FlowModelSinglePhase::DYNAMIC_VISCOSITY;
      params.set<MaterialPropertyName>("Re") = FlowModelSinglePhase::REYNOLDS_NUMBER;
      _sim.addMaterial(class_name, genName(heat_transfer.name(), "reynolds_mat"), params);
    }
    {
      const std::string class_name = "ADPrandtlNumberMaterial";
      InputParameters params = _factory.getValidParams(class_name);
      params.set<std::vector<SubdomainName>>("block") =
          heat_transfer_1phase.getFlowChannelSubdomains();
      params.set<MaterialPropertyName>("cp") =
          FlowModelSinglePhase::SPECIFIC_HEAT_CONSTANT_PRESSURE;
      params.set<MaterialPropertyName>("mu") = FlowModelSinglePhase::DYNAMIC_VISCOSITY;
      params.set<MaterialPropertyName>("k") = FlowModelSinglePhase::THERMAL_CONDUCTIVITY;
      _sim.addMaterial(class_name, genName(heat_transfer.name(), "prandtl_mat"), params);
    }
    {
      const std::string class_name = "ADWallHeatTransferCoefficient3EqnMaterial";
      InputParameters params = _factory.getValidParams(class_name);
      params.set<std::vector<SubdomainName>>("block") =
          heat_transfer_1phase.getFlowChannelSubdomains();
      params.set<MaterialPropertyName>("D_h") = FlowModelSinglePhase::HYDRAULIC_DIAMETER;
      params.set<MaterialPropertyName>("rho") = FlowModelSinglePhase::DENSITY;
      params.set<MaterialPropertyName>("vel") = FlowModelSinglePhase::VELOCITY;
      params.set<MaterialPropertyName>("v") = FlowModelSinglePhase::SPECIFIC_VOLUME;
      params.set<MaterialPropertyName>("e") = FlowModelSinglePhase::SPECIFIC_INTERNAL_ENERGY;
      params.set<MaterialPropertyName>("T") = FlowModelSinglePhase::TEMPERATURE;
      params.set<MaterialPropertyName>("T_wall") = FlowModelSinglePhase::TEMPERATURE_WALL;
      params.set<MaterialPropertyName>("p") = FlowModelSinglePhase::PRESSURE;
      params.set<MaterialPropertyName>("Re") = FlowModelSinglePhase::REYNOLDS_NUMBER;
      params.set<MaterialPropertyName>("Pr") = "Pr";
      params.set<MaterialPropertyName>("mu") = FlowModelSinglePhase::DYNAMIC_VISCOSITY;
      params.set<UserObjectName>("fp") = heat_transfer_1phase.getFluidPropertiesName();
      params.set<Real>("gravity_magnitude") = flow_channel_1phase.getGravityMagnitude();
      _sim.addMaterial(class_name, genName(heat_transfer.name(), "whtc_mat"), params);
    }
  }
}

void
Closures1PhaseFalcon::addWallFrictionChurchillMaterial(
    const FlowChannel1Phase & flow_channel) const
{
  const std::string class_name = "ADWallFrictionChurchillMaterial";
  InputParameters params = _factory.getValidParams(class_name);
  params.set<std::vector<SubdomainName>>("block") = flow_channel.getSubdomainNames();
  params.set<MaterialPropertyName>("rho") = FlowModelSinglePhase::DENSITY;
  params.set<MaterialPropertyName>("vel") = FlowModelSinglePhase::VELOCITY;
  params.set<MaterialPropertyName>("D_h") = FlowModelSinglePhase::HYDRAULIC_DIAMETER;
  params.set<MaterialPropertyName>("f_D") = FlowModelSinglePhase::FRICTION_FACTOR_DARCY;
  params.set<MaterialPropertyName>("mu") = FlowModelSinglePhase::DYNAMIC_VISCOSITY;
  params.set<Real>("roughness") = flow_channel.getParam<Real>("roughness");
  const std::string obj_name = genName(flow_channel.name(), "wall_friction_mat");
  _sim.addMaterial(class_name, obj_name, params);
  flow_channel.connectObject(params, obj_name, "roughness");
}
