//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once
#include "GeneralReporter.h"
#include "BlockRestrictable.h"
#include "MooseTypes.h"
#include "libmesh/parallel_algebra.h"
#include <unordered_map>
#include <vector>

class MooseMesh;

using namespace TIMPI;

class ClosestElemsToLine : public GeneralReporter, public BlockRestrictable
{
public:
  static InputParameters validParams();

  ClosestElemsToLine(const InputParameters & parameters);

  virtual void initialSetup() override;

  virtual void initialize() override{};
  virtual void execute() override{};
  virtual void finalize() override{};

  struct LocData
  {
    int frac_num;
    dof_id_type elem_id;
    Real distance;
    Point loc;
    LocData()
      : frac_num(libMesh::invalid_uint), elem_id(libMesh::invalid_uint), distance(0.0), loc()
    {
    }
    LocData(int frac, dof_id_type id, Real dist, const Point & p)
      : frac_num(frac), elem_id(id), distance(dist), loc(p)
    {
    }
  };

protected:
  virtual Real getDistance(const Elem & elem);
  virtual void addPoint(const Elem & elem, Real dist);
  virtual void convertVecs();
  virtual void convertToMap(std::vector<LocData> & data);

  const std::vector<Real> & _point_x1;
  const std::vector<Real> & _point_y1;
  const std::vector<Real> & _point_z1;
  const std::vector<Real> & _point_x2;
  const std::vector<Real> & _point_y2;
  const std::vector<Real> & _point_z2;
  const Real _tolerance;

  std::vector<dof_id_type> & _eid;
  std::vector<Real> & _pt_x;
  std::vector<Real> & _pt_y;
  std::vector<Real> & _pt_z;
  std::vector<Real> _dist;
  std::map<int, LocData> _dom_map;
  Point _p1;
  Point _p2;
  Point _line;

  std::string _var;
};

namespace TIMPI
{

template <>
class StandardType<ClosestElemsToLine::LocData> : public DataType
{
public:
  explicit StandardType(const ClosestElemsToLine::LocData * example = nullptr);
  StandardType(const StandardType<ClosestElemsToLine::LocData> & t);
  ~StandardType() { this->free(); }
};
} // namespace TIMPI
