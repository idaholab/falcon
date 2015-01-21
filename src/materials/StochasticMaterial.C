#include "StochasticMaterial.h"

template<>
InputParameters validParams<StochasticMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<Real>("diffusivity", "The diffusivity of the material");
  params.addRequiredParam<Real>("storage","Specific storage for the confined aquifer");
  params.addRequiredParam<Real>("init_porosity","Initial porosity of the medium");
  params.addParam<Real>("rho_w", 1.0e3,"The density of water, Kg/m^3");
  params.addParam<Real>("mu_w", 1.0e-3,"The viscosity of water, Kg/m s");
  params.addParam<Real>("gravity", 9.8,"The gravitational acceleration, m/s^2");

  params.addRequiredParam<std::vector<Real> >("mineral","Initial mineral concentration");
  params.addRequiredParam<std::vector<Real> >("molecular_weight","The molecular weight of mineral, g/mol");
  params.addRequiredParam<std::vector<Real> >("density", "The density of mineral, Kg/m^3");

  params.addCoupledVar("conductivity", "hydraulic conductivity, m/s");
  params.addCoupledVar("aperture", "The aperture size, m");

  return params;
}

StochasticMaterial::StochasticMaterial(const std::string & name,
                                 InputParameters parameters)
  :Material(name, parameters),

   // Get a parameter value for the diffusivity
   _input_diffusivity(getParam<Real>("diffusivity")),
   _input_initial_porosity(getParam<Real>("init_porosity")),
   _input_initial_Ss(getParam<Real>("storage")),
   _input_rho_w(getParam<Real>("rho_w")),
   _input_mu_w(getParam<Real>("mu_w")),
   _input_gravity(getParam<Real>("gravity")),

   _has_cond(isCoupled("conductivity")),
   _init_cond  (_has_cond ? coupledValue("conductivity") : _zero),
   _has_aptr(isCoupled("aperture")),
   _init_aptr  (_has_aptr ? coupledValue("aperture"): _zero),

   _molecular_weight(getParam<std::vector<Real> >("molecular_weight")),
   _mineral_density(getParam<std::vector<Real> >("density")),
   _input_initial_mineral(getParam<std::vector<Real> >("mineral")),

   // Declare that this material is going to have a Real valued property named "diffusivity" that Kernels can use.
   _diffusivity(declareProperty<Real>("diffusivity")),
   _Ss(declareProperty<Real>("storage")),
   _frac_stor(declareProperty<Real>("frac_storage")),
   _porosity(declareProperty<Real>("porosity")),
   _rho_w(declareProperty<Real>("rho_w")),
   _mu_w(declareProperty<Real>("mu_w")),
   _gravity(declareProperty<Real>("gravity")),
   _frac_trnsm(declareProperty<Real>("frac_transmissivity")),
   _frac_cond(declareProperty<Real>("frac_conductivity")),
   _cond(declareProperty<Real>("conductivity")),
   _aptr(declareProperty<Real>("aperture"))
   // Get stochastic conductivity values from the coupled aux variable
{
  int n = coupledComponents("v");
  _vals.resize(n);
  for (unsigned int i=0; i<_vals.size(); ++i)
    _vals[i] = &coupledValue("v", i);
}

void
StochasticMaterial::computeProperties()
{
  for(unsigned int qp=0; qp<_qrule->n_points(); ++qp)
  {

    _cond[qp] = _init_cond[qp];
    _aptr[qp] = _init_aptr[qp];
    _Ss[qp]   = _input_initial_Ss;
    _porosity[qp] = _input_initial_porosity;
    _rho_w[qp]   = _input_rho_w;
    _mu_w[qp]   = _input_mu_w;
    _gravity[qp]   = _input_gravity;

    // Compute the storage and transmissivity of a fracture
    _frac_stor[qp] = _Ss[qp] * _aptr[qp];
    _frac_trnsm[qp] = -_rho_w[qp]*_gravity[qp]*std::pow(_aptr[qp],3)/_mu_w[qp]/12;
    _frac_cond[qp] = -_rho_w[qp]*_gravity[qp]*_aptr[qp]*_aptr[qp]/_mu_w[qp]/12;

    if (_vals.size())
    {
      Real _initial_vf = 1.0;
      Real _vf = 1.0;

      for (unsigned int i=0; i<_vals.size(); ++i)
      {
        _initial_vf += 1.0e-3*_input_initial_mineral[i]*_molecular_weight[i]/_mineral_density[i];

        _vf += 1.0e-3*(*_vals[i])[qp]*_molecular_weight[i]/_mineral_density[i];

      }

      _porosity[qp] = _initial_vf *_input_initial_porosity/_vf;
    }


    if (_porosity[qp]<=1.0e-3)
      _porosity[qp]=1.0e-3;

    _diffusivity[qp] = _input_diffusivity*_porosity[qp];

  }
}
