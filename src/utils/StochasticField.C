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

#include "StochasticField.h"
#include "MooseError.h"

#include <fstream>

StochasticField::StochasticField(std::string fname)
{
  std::ifstream file(fname.c_str());
  if (!file)
    mooseError("Error opening file");

  // get the first line of the file (a comment)
  std::string tmp;
  std::getline(file, tmp);

  // get nx, ny, and nz
  std::getline(file, tmp, '=');
  file >> _nx;
  std::getline(file, tmp, '=');
  file >> _ny;
  std::getline(file, tmp, '=');
  file >> _nz;

  // get dx, dy, and dz
  std::getline(file, tmp, '=');
  file >> _dx;
  std::getline(file, tmp, '=');
  file >> _dy;
  std::getline(file, tmp, '=');
  file >> _dz;

  // reset to the next line because there may be units left on this line
  std::getline(file, tmp);

  int num_pts = _nx*_ny*_nz;
  _data.resize(num_pts);

  // We'll index this array like [z][y][x] = [(z*_ny + y)*_nx + x], so
  // read straight from the file and plot the values straight in the array.
  for (int i = 0; i < num_pts; i++)
    file >> _data[i];
}

Real
StochasticField::value(Point p)
{
  // calculate which bin the real-valued point falls into
  int x = int(p(0)/_dx);
  int y = int(p(1)/_dy);
  int z = int(p(2)/_dz);

  // check for errors in debug builds
  if (x < 0 || _nx - x <= 0)
    mooseError("Point out of bounds in StochasticField: x = ", x);
  if (y < 0 || _ny - y <= 0)
    mooseError("Point out of bounds in StochasticField: y = ", y);
  if (z < 0 || _nz - z <= 0)
    mooseError("Point out of bounds in StochasticField: z = ", z);

  return _data[(z*_ny + y)*_nx + x];
}
