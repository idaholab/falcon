#pragma once

#include "AuxKernel.h"

/** 
 * AuxKernel for computing fracture apertures based
 * on the distance from the center line of the fracture.
 * The apertures are calculated and scaled according to 
 * a generalized sigmoid function
 */
class CapsuleApertureAux : public AuxKernel
{
public:
  static InputParameters validParams();

  CapsuleApertureAux(const InputParameters & parameters);

protected:
  /** 
   * Compute the value for the aperture
   * This can be done either at the nodes, 
   * or at the gauss point
   * @return fracture aperture at queried point
   */
  virtual Real computeValue() override;

  /// @brief starting point of the line segment
  const Point & _x1;

  /// @brief ending point of the line segment
  const Point & _x2;

  /// @brief maximum initial aperture for fracture
  const Real & _aHi;

  /// @brief minimum initial aperture for fracture
  const Real & _aLo;

  /// @brief midpoint/shift of sigmoid
  const Real & _midpoint_of_sigmoid;

  /// @brief slope at midpoint of sigmoid
  const Real & _slope_at_midpoint;
};