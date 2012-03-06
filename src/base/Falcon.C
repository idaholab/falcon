/****************************************************************/
/*             DO NOT MODIFY OR REMOVE THIS HEADER              */
/*          FALCON - Fracturing And Liquid CONvection           */
/*                                                              */
/*       (c) pending 2012 Battelle Energy Alliance, LLC         */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "Moose.h"
#include "Factory.h"

//kernels
///////////////////////////////////////////////////////////////
//      solid mechanics                                      //
///////////////////////////////////////////////////////////////
#include "SolidMechXFalcon.h"
#include "SolidMechYFalcon.h"
#include "SolidMechZFalcon.h"
#include "SolidMechImplicitEuler.h"

#include "SolidMechTempCoupleXFalcon.h"

#include "SolidMechTempCoupleYFalcon.h"
#include "SolidMechTempCoupleZFalcon.h"

#include "SolidMechPoroCoupleX.h"
#include "SolidMechPoroCoupleY.h"
#include "SolidMechPoroCoupleZ.h"
#include "Gravity.h"

///////////////////////////////////////////////////////////////
//      Single phase isothermal formulation: pressure        //
///////////////////////////////////////////////////////////////
#include "PressureTimeDerivative.h"
#include "FluidFluxPressure.h"

///////////////////////////////////////////////////////////////
//      Single phase formulation: pressure & temperature     //
///////////////////////////////////////////////////////////////
#include "TemperatureTimeDerivative.h"
#include "TemperatureTimeDerivativeFluid.h"
#include "TemperatureTimeDerivativeSolid.h"
#include "TemperatureDiffusion.h"
#include "TemperatureConvection.h"

#include "MassFluxTimeDerivative_PT.h"
#include "WaterMassFluxPressure_PT.h"
#include "WaterMassFluxElevation_PT.h"

//////////////////////////////////////////////////////////////      
//       Two phase formulation: pressure & enthalpy         //
//////////////////////////////////////////////////////////////
#include "MassFluxTimeDerivative.h"
#include "WaterMassFluxPressure.h"
#include "SteamMassFluxPressure.h"
#include "WaterMassFluxElevation.h"

#include "EnthalpyTimeDerivative.h"
#include "EnthalpyDiffusion.h"
#include "EnthalpyConvectionWater.h"
#include "EnthalpyConvectionSteam.h"

////////////////////////////////////////////////////////////////
///      Souce and Sink, volume avagerged                     //
////////////////////////////////////////////////////////////////

#include "SourceSink.h"
#include "EnergyExtraction.h"

//auxkernels
#include "CoupledDensityAux_PT.h"          // water density as function of (P,T)
#include "CoupledDdensityDTAux_PT.h"       // derivative of water density to T at const P
#include "CoupledDdensityDPAux_PT.h"       // derivative of water density to P at const T

#include "CoupledTemperatureAux.h"         // T as functon of (P,H) -two phase formulation
#include "CoupledWaterSaturationAux.h"     // Sw as functon of (P,H) -two phase formulation
#include "CoupledDWaterSaturationDHAux.h"
#include "CoupledDensityAux.h"             // mixed density as functon of (P,H) -two phase formulation

#include "CoupledWaterDensityAux.h"        // water density functon of (P,H) -two phase formulation
#include "CoupledWaterViscosityAux.h"      // water viscosity functon of (P,T) - used by both PT and PH formaulations
#include "CoupledSteamDensityAux.h"        // steam density functon of (P,H) -two phase formulation
#include "CoupledSteamViscosityAux.h"      // steam viscosity functon of (P,T) - used by PH formaulations

#include "CoupledWaterEnthalpyAux.h"       //water enthalpy as function of (P,H) (MJ/kg)
#include "CoupledSteamEnthalpyAux.h"       //steam enthalpy as function of (P,H) (MJ/kg)
#include "CoupledDsteamenthalpydH_PAux.h"  //derivative of steam enthalpy to H at const P: 1 (steam only ); 0 otherwise
#include "CoupledDwaterenthalpydH_PAux.h"  //derivative of water enthalpy to H at const P: 1 (water only ); 0 otherwise

#include "CoupledDdensityDTAux.h"          //derivative of mixed density to T at const P, not used now
#include "CoupledDdensityDPAux.h"          //derivative of mixed density to P at const H,
#include "CoupledDdensityDHAux.h"          //derivative of mixed density to H at const P,
#include "CoupledDTDH_PAux.h"              //derivative of T to H at const P
#include "CoupledDTDP_HAux.h" 

#include "AnalyticalADE1D.h"
#include "VelocityAux.h"
#include "CoupledPorosityMaterialAux.h"
#include "StressStrainDamageComputeAux.h"

#include "CoupledDwaterenthalpydP_HAux.h"
#include "CoupledDsteamenthalpydP_HAux.h"

//BCs
#include "PressureNeumannBC2.h"
#include "GravityNeumannBC.h"
#include "OutFlowBC.h"
#include "OutFlowBC_PH.h"
#include "StepDirichletBC.h"
#include "StepPressureBCFunc.h"
//ICs
#include "LinearDisEnthalpyIC.h"

//materials
#include "Constant.h"
#include "PorousMedia.h"
#include "FluidFlow.h"
#include "HeatTransport.h"
#include "SolidMechanics.h"
#include "Geothermal.h"

#include "PermeabilityAux.h"
#include "ApertureAux.h"

namespace Falcon
{
  void registerObjects()
  {
    Moose::registerObjects();
    
//mechanics
    registerNamedKernel(SolidMechXFalcon, "SolidMechXFalcon");
    registerNamedKernel(SolidMechYFalcon, "SolidMechYFalcon");
    registerNamedKernel(SolidMechZFalcon, "SolidMechZFalcon");
    registerKernel(SolidMechImplicitEuler);
/*
    registerNamedKernel(SolidMechTempCoupleXFalcon, "SolidMechTempCoupleX");
    registerNamedKernel(SolidMechTempCoupleYFalcon, "SolidMechTempCoupleY");
    registerNamedKernel(SolidMechTempCoupleZFalcon, "SolidMechTempCoupleZ");
*/
    registerKernel(SolidMechTempCoupleXFalcon);
    registerKernel(SolidMechTempCoupleYFalcon);
    registerKernel(SolidMechTempCoupleZFalcon);

    registerKernel(SolidMechPoroCoupleX);
    registerKernel(SolidMechPoroCoupleY);
    registerKernel(SolidMechPoroCoupleZ);
    registerKernel(Gravity);
//isothermal flow for pressure field
    registerKernel(PressureTimeDerivative);
    registerKernel(FluidFluxPressure);  
//heat transport-PT formulation, single phase only
    registerKernel(TemperatureTimeDerivative);
    registerKernel(TemperatureTimeDerivativeFluid);
    registerKernel(TemperatureTimeDerivativeSolid);
    registerKernel(TemperatureDiffusion);
    registerKernel(TemperatureConvection);
//fluid-mass flow-single phase formulation
    registerKernel(MassFluxTimeDerivative_PT);
    registerKernel(WaterMassFluxPressure_PT);
    registerKernel(WaterMassFluxElevation_PT);
//fluid-mass flow-two phase formulation      
    registerKernel(MassFluxTimeDerivative);
    registerKernel(WaterMassFluxPressure);
    registerKernel(SteamMassFluxPressure);
    registerKernel(WaterMassFluxElevation);
//energy
    registerKernel(EnthalpyTimeDerivative);
    registerKernel(EnthalpyDiffusion);
    registerKernel(EnthalpyConvectionWater);
    registerKernel(EnthalpyConvectionSteam);

//source sink
    registerKernel(SourceSink);
    registerKernel(EnergyExtraction);  
      
//auxkernels
    registerAux(CoupledDdensityDTAux_PT);
    registerAux(CoupledDdensityDPAux_PT);    
    registerAux(CoupledDensityAux_PT);
    registerAux(CoupledWaterSaturationAux);
    registerAux(CoupledDWaterSaturationDHAux);  
    registerAux(CoupledDdensityDHAux); 
    registerAux(CoupledDTDH_PAux);
    registerAux(CoupledDTDP_HAux);
    registerAux(CoupledDdensityDPAux);
    registerAux( CoupledDdensityDTAux);
    registerAux(CoupledDensityAux);
    registerAux(CoupledWaterDensityAux);
    registerAux(CoupledWaterViscosityAux);
    registerAux(CoupledWaterEnthalpyAux);  
    registerAux(CoupledSteamDensityAux);
    registerAux(CoupledSteamViscosityAux);
    registerAux(CoupledSteamEnthalpyAux);
    registerAux(CoupledDwaterenthalpydH_PAux);
    registerAux(CoupledDsteamenthalpydH_PAux);
    registerAux(CoupledTemperatureAux);
      
    registerAux(AnalyticalADE1D);
    registerAux(VelocityAux);
    registerAux(CoupledPorosityMaterialAux);
    registerAux(StressStrainDamageComputeAux);

    registerAux(CoupledDwaterenthalpydP_HAux);
    registerAux(CoupledDsteamenthalpydP_HAux);
    
//BCs    
    registerNamedBoundaryCondition(PressureNeumannBC2, "PressureNeumannBC");
    registerBoundaryCondition(GravityNeumannBC);
    registerBoundaryCondition(OutFlowBC);
    registerBoundaryCondition(OutFlowBC_PH);
    registerBoundaryCondition(StepDirichletBC);
    registerBoundaryCondition(StepPressureBCFunc);

    // ICs
    registerInitialCondition(LinearDisEnthalpyIC);
    

//materials
    registerMaterial(Constant);
    registerMaterial(PorousMedia);
    registerMaterial(FluidFlow);
    registerMaterial(HeatTransport);
    registerMaterial(SolidMechanics);
    registerMaterial(Geothermal);
   
registerAux(PermeabilityAux);
registerAux(ApertureAux);
    
  }
}
