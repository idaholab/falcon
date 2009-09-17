#include "Moose.h"
#include "KernelFactory.h"
#include "BCFactory.h"
#include "MaterialFactory.h"
#include "AuxFactory.h"

#include "HeatConduction.h"
#include "HeatConductionImplicitEuler.h"
#include "SolidMechX.h"
#include "SolidMechY.h"
#include "SolidMechZ.h"
#include "SolidMechImplicitEuler.h"
#include "SolidMechTempCoupleX.h"
#include "SolidMechTempCoupleY.h"
#include "SolidMechTempCoupleZ.h"
#include "SolidMechSwellingSolid.h"
#include "SolidMechSwellingGas.h"
#include "SinHeat.h"
#include "PrescribedExpansion.h"
#include "Constant.h"

#include "MassInviscidFlux.h"
#include "MomentumInviscidFlux.h"
#include "MomentumViscousFlux.h"
#include "EnergyInviscidFlux.h"
#include "EnergyViscousFlux.h"
#include "GravityForce.h"
#include "GravityPower.h"
#include "Velocity.h"
#include "Temperature.h"

#include "PressureNeumannBC.h"

namespace Falcon
{
  void registerObjects()
  {
    Moose::registerObjects();
    
    KernelFactory::instance()->registerKernel<HeatConduction>("HeatConduction");
    KernelFactory::instance()->registerKernel<HeatConductionImplicitEuler>("HeatConductionImplicitEuler");
    KernelFactory::instance()->registerKernel<SolidMechX>("SolidMechX");
    KernelFactory::instance()->registerKernel<SolidMechY>("SolidMechY");
    KernelFactory::instance()->registerKernel<SolidMechZ>("SolidMechZ");
    KernelFactory::instance()->registerKernel<SolidMechImplicitEuler>("SolidMechImplicitEuler");
    KernelFactory::instance()->registerKernel<SolidMechTempCoupleX>("SolidMechTempCoupleX");
    KernelFactory::instance()->registerKernel<SolidMechTempCoupleY>("SolidMechTempCoupleY");
    KernelFactory::instance()->registerKernel<SolidMechTempCoupleZ>("SolidMechTempCoupleZ");
    KernelFactory::instance()->registerKernel<SolidMechSwellingSolid>("SolidMechSwellingSolid");
    KernelFactory::instance()->registerKernel<SolidMechSwellingGas>("SolidMechSwellingGas");
    KernelFactory::instance()->registerKernel<SinHeat>("SinHeat");
    KernelFactory::instance()->registerKernel<PrescribedExpansion>("PrescribedExpansion");

    MaterialFactory::instance()->registerMaterial<Constant>("Constant");

    KernelFactory::instance()->registerKernel<MassInviscidFlux>("MassInviscidFlux");
    KernelFactory::instance()->registerKernel<MomentumInviscidFlux>("MomentumInviscidFlux");
    KernelFactory::instance()->registerKernel<MomentumViscousFlux>("MomentumViscousFlux");
    KernelFactory::instance()->registerKernel<EnergyInviscidFlux>("EnergyInviscidFlux");
    KernelFactory::instance()->registerKernel<EnergyViscousFlux>("EnergyViscousFlux");
    KernelFactory::instance()->registerKernel<GravityForce>("GravityForce");
    KernelFactory::instance()->registerKernel<GravityPower>("GravityPower");
    KernelFactory::instance()->registerKernel<Velocity>("Velocity");
    KernelFactory::instance()->registerKernel<Temperature>("Temperature");

    BCFactory::instance()->registerBC<PressureNeumannBC>("PressureNeumannBC");
  }
}
