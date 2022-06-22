#pragma once

#include "Closures1PhaseBase.h"

/**
 * Single-phase closures for tube geometry based on TRACE v5
 */
class Closures1PhaseFalcon : public Closures1PhaseBase
{
public:
  Closures1PhaseFalcon(const InputParameters & params);

  virtual void checkFlowChannel(const FlowChannelBase & flow_channel) const override;
  virtual void checkHeatTransfer(const HeatTransferBase & heat_transfer, const FlowChannelBase & flow_channel) const override;
  virtual void addMooseObjectsFlowChannel(const FlowChannelBase & flow_channel) override;
  virtual void addMooseObjectsHeatTransfer(const HeatTransferBase & heat_transfer, const FlowChannelBase & flow_channel) override;

protected:
  /**
   * Adds material that computes wall friction factor using Churchill correlation
   *
   * @param[in] flow_channel   Flow channel component
   */
  void addWallFrictionChurchillMaterial(const FlowChannel1Phase & flow_channel) const;

public:
  static InputParameters validParams();
};
