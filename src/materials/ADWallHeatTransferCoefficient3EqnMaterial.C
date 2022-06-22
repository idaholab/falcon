#include "ADWallHeatTransferCoefficient3EqnMaterial.h"
#include "SinglePhaseFluidProperties.h"
#include "FlowModelSinglePhase.h"
#include "Numerics.h"

registerMooseObject("FalconApp", ADWallHeatTransferCoefficient3EqnMaterial);

InputParameters
ADWallHeatTransferCoefficient3EqnMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addRequiredParam<UserObjectName>("fp",
                                          "The name of the user object with fluid properties");
  params.addRequiredParam<MaterialPropertyName>("rho", "Density of the liquid");
  params.addRequiredParam<MaterialPropertyName>("vel", "x-component of the liquid velocity");
  params.addRequiredParam<MaterialPropertyName>("D_h", "Hydraulic diameter");
  params.addRequiredParam<MaterialPropertyName>("v", "Specific volume of the liquid");
  params.addRequiredParam<MaterialPropertyName>("e", "Specific internal energy of the liquid");
  params.addRequiredParam<MaterialPropertyName>("T", "Fluid temperature");
  params.addRequiredParam<MaterialPropertyName>("T_wall", "Wall temperature");
  params.addRequiredParam<MaterialPropertyName>("p", "Pressure of the liquid");
  params.addRequiredParam<MaterialPropertyName>("Re", "Reynolds number of the liquid");
  params.addRequiredParam<MaterialPropertyName>("Pr", "Prandtl number of the liquid");
  params.addRequiredParam<MaterialPropertyName>("mu", "Dynamic viscosity of the liquid");
  params.addRequiredParam<Real>("gravity_magnitude", "Gravitational acceleration magnitude");
  return params;
}

ADWallHeatTransferCoefficient3EqnMaterial::
    ADWallHeatTransferCoefficient3EqnMaterial(const InputParameters & parameters)
  : Material(parameters),
    _Hw(declareADProperty<Real>(FlowModelSinglePhase::HEAT_TRANSFER_COEFFICIENT_WALL)),
    _rho(getADMaterialProperty<Real>("rho")),
    _vel(getADMaterialProperty<Real>("vel")),
    _D_h(getADMaterialProperty<Real>("D_h")),
    _v(getADMaterialProperty<Real>("v")),
    _e(getADMaterialProperty<Real>("e")),
    _T(getADMaterialProperty<Real>("T")),
    _T_wall(getADMaterialProperty<Real>("T_wall")),
    _p(getADMaterialProperty<Real>("p")),
    _Re(getADMaterialProperty<Real>("Re")),
    _Pr(getADMaterialProperty<Real>("Pr")),
    _k(getADMaterialProperty<Real>("k")),
    _mu(getADMaterialProperty<Real>("mu")),
    _gravity_magnitude(getParam<Real>("gravity_magnitude")),
    _fp(getUserObject<SinglePhaseFluidProperties>("fp"))
{
}

const Real Nu_lam_const = 4.36;

static
ADReal
naturalConvectionLiquid(ADReal Gr, ADReal Pr)
{
  // Eq. 6-20
  ADReal turb = 0.1 * std::cbrt(Gr * Pr);
  // Eq. 6-16
  ADReal lam = 0.59 * std::pow(Gr * Pr, 0.25);
  // Eq. 6-14
  return std::max(turb, lam);
}

static
ADReal
Gnielinski(ADReal Pr, ADReal Re)
{
  // see TRACE manual p. 255
  Pr = std::max(0.5, Pr);
  // see TRACE manual p. 255
  Re = std::max(1000., Re);
  // Eq. 6-10
  ADReal f = std::pow(1.58 * std::log(Re) - 3.28, -2);
  ADReal f2 = f * 0.5;
  // Eq. 6-9
  return (f2 * (Re - 1000) * Pr) / (1 + 12.7 * std::pow(f2, 0.5) * (std::pow(Pr, 2. / 3.) - 1));
}

void
ADWallHeatTransferCoefficient3EqnMaterial::computeQpProperties()
{
  ADReal Re = std::max(1.0, _Re[_qp]);

  ADReal beta = _fp.beta_from_p_T(_p[_qp], _T[_qp]);
  // Note that Equation (6-19) in the TRACE V5.0 theory manual gives the film
  // density to be computed as
  //   T_film = 0.5 * (T_wall + _T[_qp])
  //   rho_film = rho_from_p_T(_p[_qp], T_film)
  // However, this can cause NaNs.
  // The idaholab/trace, hash 0885f96f12567a8fa5780510f48d594c35e49e2c, version
  // of TRACE computes film density as follows:
  //   rho_film = rho + 0.5 * drho/dT * (T_wall - T)
  const ADReal drho_dT = -beta * _rho[_qp]; // Using the relation: beta = -drho/dT / rho
  const ADReal rho_film = _rho[_qp] + 0.5 * drho_dT * (_T_wall[_qp] - _T[_qp]);
  ADReal dT = std::abs(_T_wall[_qp] - _T[_qp]);
  // Eq. 6-17
  ADReal Gr = std::max(THM::Grashof(beta, dT, _D_h[_qp], rho_film, _mu[_qp], _gravity_magnitude), 1.e-20);
  const ADReal rho_w = _fp.rho_from_p_T(_p[_qp], _T_wall[_qp]);
  const ADReal e_w = _fp.e_from_p_rho(_p[_qp], rho_w);
  ADReal Pr_w = THM::Prandtl(_fp.cp_from_v_e(1 / rho_w, e_w),
                           _fp.mu_from_v_e(1 / rho_w, e_w),
                           _fp.k_from_v_e(1 / rho_w, e_w));

  // turbulent flow (eq 6-12)
  ADReal Pr_ratio = _Pr[_qp] / Pr_w;
  ADReal Nu_turb = Gnielinski(_Pr[_qp], Re) * std::pow(Pr_ratio, 0.11);

  ADReal Nu_fc = std::max(Nu_lam_const, Nu_turb);

  ADReal Nu_nc = naturalConvectionLiquid(Gr, _Pr[_qp]);
  ADReal Nu = std::max(Nu_fc, Nu_nc);

  _Hw[_qp] = THM::wallHeatTransferCoefficient(Nu, _k[_qp], _D_h[_qp]);
}
