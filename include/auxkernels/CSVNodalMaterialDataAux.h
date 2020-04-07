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

#ifndef CSVNODALMATERIALDATAAUX_H
#define CSVNODALMATERIALDATAAUX_H

#include "AuxKernel.h"
#include "GeneralUserObject.h"

/** AuxKernal for reading data from CSV file
 * Creates a function that extracts data read from a CSV file,
 * via a CSVReader. It is possible to scale and add a constant to the
 * solution read.
 */
class CSVNodalMaterialDataAux : public AuxKernel
{
public:
  /** Constructor
   * @param parameters The input parameters for the kernel
   */
  CSVNodalMaterialDataAux(const InputParameters & parameters);

protected:
  /** Compute the value for the kernel
   * Computes a value for a node
   * @ return The desired value of the nodal data for the current node
   */
  virtual Real computeValue() override;

  /// Reference to the VectorPostprocessorValue storing the data
  const VectorPostprocessorValue & _vpp_data;

public:
  static InputParameters validParams();
};

#endif // CSVNODALMATERIALDATAAUX_H
