#pragma once

#include "Solver.hpp"
#include <petsctao.h>

class PetscTao : public Solver
{
public:
  PetscTao(Problem * problem, hit::Node * params);
  ~PetscTao();

  int solve() override;

protected:
  static PetscErrorCode formFunctionGradient(Tao, Vec, PetscReal *, Vec, void *);
  static PetscErrorCode formHessian(Tao, Vec, Mat, Mat, void *);

private:
  registerObjectHpp(PetscTao);

  std::vector<std::string> _petsc_options;
  std::vector<std::string> _petsc_option_names;
  std::vector<std::string> _petsc_option_values;

  PetscErrorCode _ierr;
  PetscMPIInt _size;
  std::vector<DofId> _dofs;
  PetscInt _ndof;
  Tao _tao;
  Vec _x;
  Vec _lb;
  Vec _ub;
  Mat _H;
  const PetscReal * _solution;
};
