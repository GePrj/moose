#ifndef RECONVARIC_H
#define RECONVARIC_H

#include "InitialCondition.h"
#include "EBSDReader.h"

// Forward Declarations
class ReconVarIC;

template<>
InputParameters validParams<ReconVarIC>();

/**
 * ReconVarIC creates a polycrystal initial condition from an EBSD dataset

*/
class ReconVarIC : public InitialCondition
{
public:

  ReconVarIC(const std::string & name, InputParameters parameters);

  virtual void initialSetup();
  virtual Real value(const Point &);

private:
  MooseMesh & _mesh;
  NonlinearSystem & _nl;
  const EBSDReader & _ebsd_reader;

  unsigned int _op_num;
  unsigned int _grain_num;
  unsigned int _op_index;

  Point _bottom_left;
  Point _top_right;
  Point _range;

  struct GrainPoint {
    unsigned int _grn;
    Real _x, _y, _z; // TODO: Use Point
  };
  std::map<unsigned int, GrainPoint> _gp;

  std::vector<Real> _sum_x, _sum_y, _sum_z;
  std::vector<Point> _centerpoints;
  std::vector<Real> _assigned_op;
};

#endif //RECONVARIC_H
