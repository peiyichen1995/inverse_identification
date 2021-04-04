#include "PetscTao.hpp"
#include "Objective.hpp"

registerObjectCpp(PetscTao);

PetscTao::PetscTao(Problem * problem, hit::Node * params) : Solver(problem, params)
{
  _ierr = PetscInitialize(PETSC_NULL, PETSC_NULL, PETSC_NULL, PETSC_NULL);
  if (_ierr)
    exit(_ierr);
}

PetscTao::~PetscTao()
{
  _ierr = PetscFinalize();
  if (_ierr)
    exit(_ierr);
}

int
PetscTao::solve()
{
  MPI_Comm_size(PETSC_COMM_WORLD, &_size);
  if (_size != 1)
    SETERRQ(PETSC_COMM_WORLD, PETSC_ERR_WRONG_MPI_SIZE, "Only serial is supported for now.");

  _dofs = _problem->dofs();
  _ndof = _dofs.size();

  _ierr = VecCreateSeq(PETSC_COMM_WORLD, _ndof, &_x);
  _ierr = MatCreateSeqAIJ(PETSC_COMM_WORLD, _ndof, _ndof, _ndof, nullptr, &_H);

  _ierr = TaoCreate(PETSC_COMM_WORLD, &_tao);
  _ierr = TaoSetType(_tao, TAOBNTL);

  /* Check for TAO command line options */
  _ierr = PetscOptionsSetValue(NULL, "-tao_monitor", NULL);
  _ierr = TaoSetFromOptions(_tao);

  /* Set solution vec and an initial guess */
  std::vector<PetscReal> dof_values = _problem->dofValues();
  _ierr = VecSetValues(_x, _ndof, &_dofs[0], &dof_values[0], INSERT_VALUES);
  _ierr = TaoSetInitialVector(_tao, _x);

  /* Set routines for function, gradient, hessian evaluation */
  _ierr = TaoSetObjectiveAndGradientRoutine(_tao, formFunctionGradient, _problem);
  _ierr = TaoSetHessianRoutine(_tao, _H, _H, formHessian, _problem);

  /* Set bounds */
  // _ierr = VecCreateSeq(PETSC_COMM_WORLD, 7, &lb);
  // _ierr = VecCreateSeq(PETSC_COMM_WORLD, 7, &ub);
  // _ierr = VecSet(lb, 0);
  // _ierr = VecSetValues(ub, 7, ind, ub_vals, INSERT_VALUES);
  // _ierr = TaoSetVariableBounds(tao, lb, ub);

  /* SOLVE THE APPLICATION */
  _ierr = TaoSolve(_tao);

  /* Print solution */
  _ierr = TaoGetSolutionVector(_tao, &_x);
  _ierr = VecGetArrayRead(_x, &_solution);
  for (PetscInt i = 0; i < _ndof; i++)
    std::cout << _solution[i] << std::endl;
  _ierr = VecRestoreArrayRead(_x, &_solution);

  /* Clean up */
  _ierr = TaoDestroy(&_tao);
  _ierr = VecDestroy(&_x);
  _ierr = MatDestroy(&_H);

  return _ierr;
}

PetscErrorCode
PetscTao::formFunctionGradient(Tao /*Tao solver = */ tao,
                               Vec /* solution vector =  */ X,
                               PetscReal * /* objective function = */ f,
                               Vec /* gradient = */ G,
                               void * /*context = */ context)
{
  PetscErrorCode ierr;
  Problem * problem = static_cast<Problem *>(context);
  PetscInt ndof = problem->dofs().size();

  /* Get pointers to vector data */
  const PetscReal * x;
  ierr = VecGetArrayRead(X, &x);
  PetscReal * g;
  ierr = VecGetArray(G, &g);

  /* Compute f(X) */
  problem->setDofValues(x);
  ADReal objective = problem->objective()->value();
  *f = raw_value(objective);

  /* Compute G(X) */
  Vector gradient = problem->gradient(objective);
  for (PetscInt i = 0; i < ndof; i++)
    g[i] = gradient[i];

  /* Restore vectors */
  ierr = VecRestoreArrayRead(X, &x);
  ierr = VecRestoreArray(G, &g);

  return ierr;
}

PetscErrorCode
PetscTao::formHessian(Tao /*Tao solver = */ tao,
                      Vec /* solution vector =  */ X,
                      Mat /* Hessian = */ H,
                      Mat /* preconditioner = */ Hpre,
                      void * /*context = */ context)
{
  PetscErrorCode ierr;
  Problem * problem = static_cast<Problem *>(context);
  std::vector<PetscInt> dofs = problem->dofs();
  PetscInt ndof = dofs.size();
  PetscReal v[ndof][ndof];
  PetscBool assembled;

  /* Zero existing matrix entries */
  ierr = MatAssembled(H, &assembled);
  if (assembled)
    ierr = MatZeroEntries(H);

  /* Get a pointer to vector data */
  const PetscReal * x;
  ierr = VecGetArrayRead(X, &x);

  /* Compute H(X) entries */
  problem->setDofValues(x);
  ADReal objective = problem->objective()->value();
  Matrix Hessian = problem->Hessian(objective);
  for (PetscInt i = 0; i < ndof; i++)
    for (PetscInt j = 0; j < ndof; j++)
      v[i][j] = Hessian(i, j);
  ierr = MatSetValues(H, ndof, &dofs[0], ndof, &dofs[0], v[0], INSERT_VALUES);

  ierr = VecRestoreArrayRead(X, &x);

  /* Assemble matrix */
  ierr = MatAssemblyBegin(H, MAT_FINAL_ASSEMBLY);
  ierr = MatAssemblyEnd(H, MAT_FINAL_ASSEMBLY);

  return ierr;
}
