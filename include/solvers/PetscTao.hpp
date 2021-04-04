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

  PetscErrorCode _ierr;
  PetscMPIInt _size;
  std::vector<DofId> _dofs;
  PetscInt _ndof;
  Tao _tao;
  Vec _x;
  Vec _lb;
  Vec _ub;
  Mat _H;
  // PetscReal ub_vals[] = {PETSC_INFINITY,
  //                        PETSC_INFINITY,
  //                        PETSC_INFINITY,
  //                        PETSC_INFINITY,
  //                        PETSC_INFINITY,
  //                        PETSC_INFINITY,
  //                        1};
  const PetscReal * _solution;
};
