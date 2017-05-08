/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c)     2012 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

/*----------------------------------------------------------------

Contributors: Yidong Xia (INL)

Descriptions: material properties for pressure-temperature based
              T-H-M-C balance equations

----------------------------------------------------------------*/


#include "PTGeothermal.h"

// libMesh includes
#include "libmesh/quadrature.h"

template<>
InputParameters validParams<PTGeothermal>()
{
  InputParameters params = validParams<Material>();

  MooseEnum stat("constant compressibility wseos", "constant");
  params.addParam<MooseEnum>("fluid_property_formulation", stat,
  "Fluid property formulation, default = constant");

  MooseEnum stabilizer("none zero supg supg_dc", "none");
  params.addParam<MooseEnum>("stabilizer", stabilizer,
  "Energy transport stabilizer, default = none");

  MooseEnum perm_func("constant 1 2", "constant");
  params.addParam<MooseEnum>("permeability_function_type", perm_func,
  "Type for permeability function, default = constant");

  params.addParam<Real>(
  "initial_fluid_pressure", 0.0,
  "Initial fluid pressure [Pa], default = 0");

  params.addParam<Real>(
  "total_overburden_confining_pressure", 45.0e6,
  "Total overburden confining pressure [Pa], default = 45000000");

  params.addParam<Real>(
  "fitting_parameter", 10.0,
  "Fitting parameter, default = 10");

  params.addParam<Real>(
  "reference_pressure", 101325,
  "Reference pressure [Pa], default = 101325");

  params.addParam<Real>(
  "reference_temperature", 297.15,
  "Reference temperature [K], default = 297.15");

  params.addParam<Real>(
  "permeability", 1.0e-12,
  "Intrinsic permeability [m^2], default = 1.0e-12");

  params.addParam<Real>(
  "porosity", 0.3,
  "Rock porosity, default = 0.3");

  params.addParam<Real>(
  "compressibility", 1.0e-5,
  "Total compressibility of the researvoir, default = 1.0e-5");

  params.addParam<Real>(
  "density_rock", 2.50e3,
  "Rock density [kg/m^3]");

  params.addParam<Real>(
  "gravity", 9.80665,
  "Gravity acceleration constant [m/s^2], default = 9.80665");

  params.addParam<RealGradient>(
  "gravity_direction", RealGradient(0,0,-1),
  "Gravity unit directional vector, default = '0 0 -1'");

  params.addParam<RealGradient>(
  "constant_pressure_gradient", RealGradient(0,0,0),
  "Constant pressure gradient, default = '0 0 0'");

  params.addCoupledVar(
  "pressure",
  "Assign nonlinear variable for pressure [Pa], if mass balance is involved");

  params.addCoupledVar(
  "temperature",
  "Assign nonlinear variable for temperature [K], if energy balance is involved");

  params.addCoupledVar(
  "stochastic_permeability",
  "Assign aux variable for stochastic permeability [m2]");

  params.addParam<Real>(
  "density_water", 1000,
  "Initial water density [kg/m^3], default = 1000");

  params.addParam<Real>(
  "viscosity_water", 0.001,
  "Initial water viscosity [Pa.s], default = 0.001");

  params.addParam<Real>(
  "specific_heat_rock", 0.92e3,
  "Specific heat of the rock [J/(kg.K)], default = 0.92e3");

  params.addParam<Real>(
  "specific_heat_water", 4.186e3,
  "Specific heat of water [J/(kg.K)], default = 4.186e3");

  params.addParam<Real>(
  "thermal_conductivity", 2.5,
  "Thermal conductivity of the reservoir [W/(m.K)], default = 2.5");

  params.addParam<Real>(
  "supg_dc_threshold", 1e-12,
  "Threshold magnitude of temperature gradient to include SUPG discontinuity capturing, default = 1e-12");

  return params;
}


/*******************************************************************************
Routine: PTGeothermal -- constructor
Authors: Yidong Xia
*******************************************************************************/
PTGeothermal::PTGeothermal(const InputParameters & parameters):
  Material(parameters),

  _istat(getParam<MooseEnum>("fluid_property_formulation")),
  _istab(getParam<MooseEnum>("stabilizer")),

  // ========================================================
  // flags to indicate the involvement of terms and equations
  // ========================================================
  _has_pres(isCoupled("pressure")),
  _has_temp(isCoupled("temperature")),
  _sto_perm(isCoupled("stochastic_permeability")),

  _perm_func(getParam<MooseEnum>("permeability_function_type")),

  // =====================
  // user-input parameters
  // =====================
  _ipini(getParam<Real>("initial_fluid_pressure")),
  _iptoc(getParam<Real>("total_overburden_confining_pressure")),
  _icfit(getParam<Real>("fitting_parameter")),
  _ipres(getParam<Real>("reference_pressure")),
  _itemp(getParam<Real>("reference_temperature")),
  _iperm(getParam<Real>("permeability")),
  _iporo(getParam<Real>("porosity")),
  _irrho(getParam<Real>("density_rock")),
  _irsph(getParam<Real>("specific_heat_rock")),
  _icomp(getParam<Real>("compressibility")),
  _iwrho(getParam<Real>("density_water")),
  _iwvis(getParam<Real>("viscosity_water")),
  _iwsph(getParam<Real>("specific_heat_water")),
  _ithco(getParam<Real>("thermal_conductivity")),
  _ipgdc(getParam<Real>("supg_dc_threshold")),
  _igfor(getParam<Real>("gravity")),

  _igvec(getParam<RealGradient>("gravity_direction")),
  _igrdp(getParam<RealGradient>("constant_pressure_gradient")),

  // ========================
  // main nonlinear variables
  // ========================
  // coupled solution variable values
  _pres(_has_pres ? coupledValue("pressure")    : _zero),
  _temp(_has_temp ? coupledValue("temperature") : _zero),

  _pres_old((_has_pres & _is_transient) ? coupledValueOld("pressure") : _zero),
  _temp_old((_has_temp & _is_transient) ? coupledValueOld("temperature") : _zero),

  // coupled solution variable gradients
  _grad_pres(_has_pres ? coupledGradient("pressure")    : _grad_zero),
  _grad_temp(_has_temp ? coupledGradient("temperature") : _grad_zero),

  _perm_sto(_sto_perm ? coupledValue("stochastic_permeability") : _zero),

  // ===================
  // material properties
  // ===================
  _stat(declareProperty<unsigned int>("fluid_property_formulation")),
  _stab(declareProperty<unsigned int>("stabilization_options")),

  _perm(declareProperty<Real>("permeability")),
  _poro(declareProperty<Real>("porosity")),
  _rrho(declareProperty<Real>("density_rock")),
  _rsph(declareProperty<Real>("specific_heat_rock")),
  _comp(declareProperty<Real>("compressibility")),
  _wrho(declareProperty<Real>("density_water")),
  _wvis(declareProperty<Real>("viscosity_water")),
  _wtau(declareProperty<Real>("tau_water")),
  _wsph(declareProperty<Real>("specific_heat_water")),
  _thco(declareProperty<Real>("thermal_conductivity")),
  _gfor(declareProperty<Real>("gravity")),
  _epor(declareProperty<Real>("porous_media_energy")),
  _drop(declareProperty<Real>("partial_rho_over_partial_pres")),
  _drot(declareProperty<Real>("partial_rho_over_partial_temp")),
  _dkdp(declareProperty<Real>("partial_perm_over_partial_pres")),
  _tau1(declareProperty<Real>("supg_tau1")),
  _tau2(declareProperty<Real>("supg_tau2")),

  _guvec(declareProperty<RealGradient>("gravity_direction")),
  _wdflx(declareProperty<RealGradient>("darcy_flux_water")),
  _wdmfx(declareProperty<RealGradient>("darcy_mass_flux_water")),
  _evelo(declareProperty<RealGradient>("energy_convective_velocity")),

  // ============================
  // stateful material properties
  // ============================
  _wrho_old(declarePropertyOld<Real>("density_water"))
{}

/*******************************************************************************
Routine: initQpStatefulProperties -- called only when stateful props are used
Authors: Yidong Xia
*******************************************************************************/
void
PTGeothermal::initQpStatefulProperties()
{
  _wrho[_qp] = _iwrho;
}

/*******************************************************************************
Routine: computeProperties -- self-explanatory
Authors: Yidong Xia
*******************************************************************************/
void
PTGeothermal::computeProperties()
{
  for(_qp=0; _qp<_qrule->n_points(); _qp++)
    computeQpProperties();
}

/*******************************************************************************
Routine: computeQpProperties -- self-explanatory
Authors: Yidong Xia
*******************************************************************************/
void
PTGeothermal::computeQpProperties()
{
  // assign user-input values or keep default

  // scalar arrays
  _stat[_qp] = _istat;
  _stab[_qp] = _istab;
  _perm[_qp] = _iperm;
  _poro[_qp] = _iporo;
  _rrho[_qp] = _irrho;
  _rsph[_qp] = _irsph;
  _comp[_qp] = _icomp;
  _wrho[_qp] = _iwrho;
  _wvis[_qp] = _iwvis;
  _wsph[_qp] = _iwsph;
  _thco[_qp] = _ithco;
  _gfor[_qp] = _igfor;
  _epor[_qp] = 0.0;
  _drop[_qp] = 0.0;
  _drot[_qp] = 0.0;
  _dkdp[_qp] = 0.0;
  _tau1[_qp] = 0.0;
  _tau2[_qp] = 0.0;

  // vector arrays
  _guvec[_qp] = _igvec;
        gradp = _igrdp;

  // use the nonlinear variables when available
  rpres = _ipres; if (_has_pres) rpres = _pres[_qp];
  rtemp = _itemp; if (_has_temp) rtemp = _temp[_qp];
  rpres_old = _ipres; if (_has_pres) rpres_old = _pres_old[_qp];
  rtemp_old = _itemp; if (_has_temp) rtemp_old = _temp_old[_qp];

  // use stochastic permeability when available
  if (_sto_perm)
    _perm[_qp] = _perm_sto[_qp];

  if (_has_pres)
  {
    gradp = _grad_pres[_qp];

    if (_perm_func == 0)
    {
      // constant permeability
    }
    else if (_perm_func == 1)
    {
      // type-1 permeability function

      _perm[_qp] = _iperm * std::exp(_icfit * (rpres - _ipini) / _iptoc);
      _dkdp[_qp] = _icfit / _iptoc * _perm[_qp];
    }
    else if (_perm_func == 2)
    {
      // type-2 permeability function

      Real a =  0.2003;
      Real d = -0.22252;
      Real c = -0.03394;
      Real P = rpres / 1E6;

      Real part1 = a + c * std::sqrt(P);
      Real part2 = 1.0 + d * std::sqrt(P);
      Real part3 = part1 / part2;

      _perm[_qp] = _iperm * std::pow(part3, 2.0/3.0);
      _dkdp[_qp] = 0.0;

      /* use of analytical Jacobian and numerical Jacobian leads to divergence */

      /* analytical Jacobian */

      //_dkdp[_qp] = _iperm * (1.0/3.0) * std::pow(part3, (-1.0/3.0)) * (c-d*part3)/std::sqrt(P)/part2;

      /* numerical Jacobian */

      //Real eps = 1e-3;
      //Real Pd = P - eps;
      //Real part1d = a + c * std::sqrt(Pd);
      //Real part2d = 1.0 + d * std::sqrt(Pd);
      //Real part3d = part1d / part2d;
      //Real permd = _iperm * std::pow(part3d, 2.0/3.0);
      //_dkdp[_qp] = (_perm[_qp] - permd) / eps;
    }
    else
      mooseError("Unknown permeability function!");
  }

  // options for calculating variable water properties

  if (_stat[_qp] == 1) // solely T-dependent fluid properties and use compressibility
  {
    _wrho[_qp] = computeTempBasedWaterDens(rtemp);
    _wvis[_qp] = computeTempBasedWaterVisc(rtemp);
    _drot[_qp] = computeTempBasedWaterPartialDensOverPartialTemp(rtemp);
    _drop[_qp] = _wrho[_qp]*_comp[_qp];

  }
  else if (_stat[_qp] == 2) // P-T dependent fluid properties
  {
    Real inp[2] = {rpres, rtemp};
    Real out[2] = {0.0, 0.0};

    Real inpd[2][2] = { {1.0, 0.0}, {0.0, 1.0} };
    Real outd[2][2] = { {0.0, 0.0}, {0.0, 0.0} };

    computeWaterEquationOfStatePT_dv(inp, inpd, out, outd, 2);

    _wrho[_qp] = out[0];
    _drop[_qp] = outd[0][0];
    _drot[_qp] = outd[0][1];

    computeViscosity(_wrho[_qp], rtemp, _wvis[_qp]);
  }

  // water mobility
  _wtau[_qp] = _perm[_qp]/_wvis[_qp];

  // compute the following vectors and use them in kernels and aux kernels
  _wdflx[_qp] = -_wtau[_qp]*(gradp-_wrho[_qp]*_gfor[_qp]*_guvec[_qp]);
  _wdmfx[_qp] =  _wrho[_qp]*_wdflx[_qp];
  _evelo[_qp] =  _wsph[_qp]*_wdmfx[_qp];

  if (_has_temp)
  {
    // to be used in both energy time derivative and residual kernels
    if (_is_transient)
      _epor[_qp] = (1.0-_poro[_qp])*_rrho[_qp]*_rsph[_qp]
                       + _poro[_qp]*_wrho[_qp]*_wsph[_qp];

    // pre-compute a few varialbes upon stabilization options
    if (_stab[_qp] == 1)
    {
      _evelo[_qp] = 0.0;
    }
    else if (_stab[_qp] == 2)
    {
      // Streamline Upwind Petrov Galerkin

      // compute the SUPG h size
      const double hsupg = _current_elem->hmin();

      // compute the energy convective velocity magnitude
      Real amag = _wsph[_qp]*sqrt(_wdmfx[_qp](0)*_wdmfx[_qp](0)+
                                  _wdmfx[_qp](1)*_wdmfx[_qp](1)+
                                  _wdmfx[_qp](2)*_wdmfx[_qp](2));

      // compute the SUPG stabilization parameter: tau1
      _tau1[_qp] = hsupg / (2.0*(amag+1.0e-7));
    }
    else if (_stab[_qp] == 3)
    {
      // Streamline Upwind Petrov Galerkin with Discontinuity Capturing
      // (Note: this method may only slightly improve stability
      //        or make it worse depending on specific situations)
      // Specifically speaking, the nonlinear convergence becomes worse
      // So always use with caution and only when you understand the mechanism

      // compute the SUPG h size
      const double hsupg = _current_elem->hmin();

      // compute the energy convective velocity magnitude
      Real amag = _wsph[_qp]*sqrt(_wdmfx[_qp](0)*_wdmfx[_qp](0)+
                                  _wdmfx[_qp](1)*_wdmfx[_qp](1)+
                                  _wdmfx[_qp](2)*_wdmfx[_qp](2));

      // compute the SUPG stabilization parameter: tau1
      _tau1[_qp] = hsupg / (2.0*(amag+1.0e-7));

      // compute the magnitude of temperature gradient
      Real grad_temp_mod = sqrt( _grad_temp[_qp](0)+_grad_temp[_qp](0)
                                +_grad_temp[_qp](1)+_grad_temp[_qp](1)
                                +_grad_temp[_qp](2)+_grad_temp[_qp](2));

      // compute the discontinuity capturing parameter: tau2
      if (grad_temp_mod > _ipgdc) _tau2[_qp] = hsupg / (2.0*grad_temp_mod);
    }
  }
}


/*******************************************************************************
Routine: computeWaterEquationOfStatePT_dv
         --- compute water density and enthalpy, as well as the Jacobian matrix
Authors: Yidong Xia

Hint: 2 should be the maximum number of differentiation directions

Differentiation of computeWaterEquationOfStatePT in forward (tangent) mode:
  variations   of useful results: out[0:2-1]
  with respect to varying inputs: inp[0:2-1]
  RW status of diff variables: out[0:2-1]:out inp[0:2-1]:in
  Plus diff mem management of: out:in inp:in
*******************************************************************************/
void
PTGeothermal::
computeWaterEquationOfStatePT_dv(Real inp[2], Real inpd[2][2],
                                 Real out[2], Real outd[2][2],
                                 int nbdirs)
{
    Real pres;
    Real temp;
    Real dens;
    Real enth;
    Real rconst;
    Real pstar;
    Real tstar;
    Real gamma_pie;
    Real gamma_tau;
    Real pie;
    Real tau;
    Real presd[2];
    Real tempd[2];
    Real densd[2];
    Real enthd[2];
    Real gamma_pied[2];
    Real gamma_taud[2];
    Real pied[2];
    Real taud[2];
    int nd;
    int ii1;
    temp = inp[1];
    pstar = 16.53e6;
    tstar = 1386.e0;
    for (nd = 0; nd < nbdirs; ++nd) {
        presd[nd] = inpd[0][nd];
        tempd[nd] = inpd[1][nd];
        pied[nd] = presd[nd]/pstar;
        taud[nd] = -(tstar*tempd[nd]/(temp*temp));
    }
    pres = inp[0];
    dens = 0.0;
    enth = 0.0;
    rconst = 0.461526e3;
    gamma_pie = 0.0;
    gamma_tau = 0.0;
    pie = pres/pstar;
    tau = tstar/temp;
    /// shifted value of temperature, =T*/T
    /// n coefficients of region 1 gibbs eq.
    Real n[34] = {0.14632971213167e+00, -0.84548187169114e+00, -
        0.37563603672040e+01, 0.33855169168385e+01, -0.95791963387872e+00,
        0.15772038513228e+00, -0.16616417199501e-01, 0.81214629983568e-03,
        0.28319080123804e-03, -0.60706301565874e-03, -0.18990068218419e-01, -
        0.32529748770505e-01, -0.21841717175414e-01, -0.52838357969930e-04, -
        0.47184321073267e-03, -0.30001780793026e-03, 0.47661393906987e-04, -
        0.44141845330846e-05, -0.72694996297594e-15, -0.31679644845054e-04, -
        0.28270797985312e-05, -0.85205128120103e-09, -0.22425281908000e-05, -
        0.65171222895601e-06, -0.14341729937924e-12, -0.40516996860117e-06, -
        0.12734301741641e-08, -0.17424871230634e-09, -0.68762131295531e-18,
        0.14478307828521e-19, 0.26335781662795e-22, -0.11947622640071e-22,
        0.18228094581404e-23, -0.93537087292458e-25};
    /// I coefficients of region 1 gibbs eq.
    Real I[34] = {0., 0., 0., 0., 0., 0., 0., 0., 1., 1., 1., 1., 1., 1., 2.
        , 2., 2., 2., 2., 3., 3., 3., 4., 4., 4., 5., 8., 8., 21., 23., 29.,
        30., 31., 32.};
    /// J coefficients of region 1 gibbs eq.
    Real J[34] = {-2., -1., 0., 1., 2., 3., 4., 5., -9., -7., -1., 0., 1.,
        3., -3., 0., 1., 3., 17., -4., 0., 6., -5., -2., 10., -8., -11., -6.,
        -29., -31., -38., -39., -40., -41.};
    Real arg1;
    Real arg1d[2];
    Real arg2;
    Real arg2d[2];
    for (nd = 0; nd < nbdirs; ++nd) {
        gamma_pied[nd] = 0.0;
        gamma_taud[nd] = 0.0;
    }
    /// Gibbs free energy terms for region 1
    for (int i = 0; i < 34; ++i) {
        arg1 = (I[i]-1.0)*log(7.1-pie);
        arg2 = J[i]*log(tau-1.222);
        for (nd = 0; nd < nbdirs; ++nd) {
            arg1d[nd] = -((I[i]-1.0)*pied[nd]/(7.1-pie));
            arg2d[nd] = J[i]*taud[nd]/(tau-1.222);
            gamma_pied[nd] = gamma_pied[nd] - n[i]*I[i]*(arg1d[nd]*exp(arg1)*
                exp(arg2)+exp(arg1)*arg2d[nd]*exp(arg2));
            arg1d[nd] = -(I[i]*pied[nd]/(7.1-pie));
            arg2d[nd] = (J[i]-1.0)*taud[nd]/(tau-1.222);
        }
        gamma_pie -= n[i]*I[i]*exp((I[i]-1.0)*log(7.1-pie))*exp(J[i]*log(tau-
        1.222));
        arg1 = I[i]*log(7.1-pie);
        arg2 = (J[i]-1.0)*log(tau-1.222);
        for (nd = 0; nd < nbdirs; ++nd)
            gamma_taud[nd] = gamma_taud[nd] + n[i]*J[i]*(arg1d[nd]*exp(arg1)*
                exp(arg2)+exp(arg1)*arg2d[nd]*exp(arg2));
        gamma_tau += n[i]*J[i]*exp(I[i]*log(7.1-pie))*exp((J[i]-1.0)*log(tau-
        1.222));
    }
    dens = pstar/(rconst*temp*gamma_pie);
    for (nd = 0; nd < nbdirs; ++nd) {
        /// Density
        densd[nd] = -(pstar*rconst*(tempd[nd]*gamma_pie+temp*gamma_pied[nd])/(
            rconst*temp*gamma_pie*(rconst*temp*gamma_pie)));
        /// Enthalpy
        enthd[nd] = rconst*(tempd[nd]*(tau*gamma_tau-pie*gamma_pie)+temp*(taud
            [nd]*gamma_tau+tau*gamma_taud[nd]-pied[nd]*gamma_pie-pie*
            gamma_pied[nd])) + (presd[nd]*dens-pres*densd[nd])/(dens*dens);
        /// Outputs
        for (ii1 = 0; ii1 < 2; ++ii1)
            outd[ii1][nd] = 0.0;
        outd[0][nd] = densd[nd];
    }
    enth = rconst*temp*(tau*gamma_tau-pie*gamma_pie) + pres/dens;
    out[0] = dens;
    for (nd = 0; nd < nbdirs; ++nd)
        outd[1][nd] = enthd[nd];
    out[1] = enth;
}


/*******************************************************************************
Routine: computeViscosity
         --- compute water viscosity based on density and temperature
Authors: Yidong Xia
*******************************************************************************/
void
PTGeothermal::
computeViscosity(Real dens, Real temp, Real& visc)
{
  /// Calculates dynamic viscosity of water or steam, given the density
  /// and temperature, using the IAPWS industrial formulation 2008.
  /// Critical enhancement of viscosity near the critical point is not included.

  /// Scalars
  Real dstar  = 322.000e+00; /// density shifting term, d*
  Real tstar  = 647.096e+00; /// temperature shifting term, T*
  Real mustar =   1.000e-06; /// viscosity shifting term, mu*
  Real del    = dens/dstar;  /// shifted value of density, =d/d*
  Real tau    = temp/tstar;  /// shifted value of temperature, =T/T*

  /// I exponents of summation 1 term
  Real I[21] = {0.,1.,2.,3.,0.,1.,2.,3.,5.,0.,1.,2.,3.,4.,0.,1.,0.,3.,4.,3.,5.};

  /// J exponents of summation 1 term
  Real J[21] = {0.,0.,0.,0.,1.,1.,1.,1.,1.,2.,2.,2.,2.,2.,3.,3.,4.,4.,5.,6.,6.};

  /// H ceofficients for viscosity and summation 0 term
  Real H0[4] = {1.67752e0, 2.20462e0, 0.6366564e0, -0.241605e0};

  /// H coefficients for viscosity and summation 1 term
  Real H1[21] = { 5.20094e-01,  8.50895e-02, -1.08374e+00, -2.89555e-01,  2.22531e-01,
                  9.99115e-01,  1.88797e+00,  1.26613e+00,  1.20573e-01, -2.81378e-01,
                 -9.06851e-01, -7.72479e-01, -4.89837e-01, -2.57040e-01,  1.61913e-01,
                  2.57399e-01, -3.25372e-02,  6.98452e-02,  8.72102e-03, -4.35673e-03,
                 -5.93264e-04};

  /// Viscosity in dilute-gas limit:
  Real summation0 = 0.0;
  for (int i=0; i<4; i++)
    summation0 += H0[i]/pow(tau,i);

  /// First viscosity term due to dilute-gas limit
  Real mu0 = 100.e0*sqrt(tau)/summation0;

  /// Contribution due to finite density:
  Real summation1 = 0.0;
  for (int i=0; i<21; i++)
    summation1 += pow((1/tau-1),I[i])*H1[i]*pow((del-1),J[i]);

  /// Second viscosity term due to finite density
  Real mu1 = exp(del*summation1);

  visc = mu0*mu1*mustar;
}



/*******************************************************************************
Routine: computeTempBasedWaterDens
         --- compute temperature based water density
Authors: Yidong Xia
*******************************************************************************/
Real
PTGeothermal::computeTempBasedWaterDens(Real temp)
{
  return
  1e3*(1.0-pow(temp-277.1363,2)/508929.2*(temp+15.7914)/(temp-205.02037));
}


/*******************************************************************************
Routine: computeTempBasedWaterVisc
         --- compute temperature based water viscosity
Authors: Yidong Xia
*******************************************************************************/
Real
PTGeothermal::computeTempBasedWaterVisc(Real temp)
{
  if (temp>=273.15 && temp<=313.15)
    return 1.787e-3*exp((-0.03288+1.962e-4*(temp-273.15))*(temp-273.15));
  else if (temp>313.15 && temp<=373.15)
    return 0.001*pow(1.0+0.015512*(temp-293.15),-1.572);
  else if (temp>373.15 && temp<=573.15)
    return 0.2414*pow(10.0,247.8/(temp-140)-4.0);
  else
  {
    _console<<"temp = "<<temp-273.15<<" Celsius degree"<<std::endl;
    mooseError("Temperature out of [0, 300] Celsius degree \
               in temperature dependent fluid properties");
  }
  return 0.0;
}


/*******************************************************************************
Routine: computeTempBasedWaterPartialDensOverPartialTemp
         --- compute temperature based partial density over partial temperature
Authors: Yidong Xia
*******************************************************************************/
Real
PTGeothermal::computeTempBasedWaterPartialDensOverPartialTemp(Real temp)
{
  return
  -1e3*(
          2.0*(temp-277.1363)/508929.2*(temp+15.7914)/(temp-205.02037)
        + pow(temp-277.1363,2)/508929.2
          *(1.0/(temp-205.02037)-(temp+15.7914)/pow((temp-205.02037),2))
       );
}
