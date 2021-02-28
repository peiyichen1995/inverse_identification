#include "TissueParameterIdentification.hpp"
#include "PeggyModel.hpp"
#include "Utils.hpp"

#include <cstdlib>

int
main(int argc, char const * argv[])
{
  auto problem = new TissueParameterIdentification();

  // Add variables
  problem->addVariable("mu_4");
  problem->addVariable("beta_4");
  problem->addVariable("rho");

  // Add auxiliary variables
  problem->addAuxVariable("lambda_1");
  problem->addAuxVariable("lambda_2");
  problem->addAuxVariable("tissue_angle");

  // Add hyperelastic model
  auto model = new PeggyModel(problem);
  problem->addHyperelasticModel(model);

  // Add experimental data
  int num_data_files = atoi(argv[1]);
  for (int i = 2; i < num_data_files + 2; i++)
    problem->addExperimentalData(argv[i]);

  // Initialize variables
  problem->init();

  problem->printInfo();
  problem->printVariableValues();
  problem->printExperimentalData();

  Vector x(3);
  x[0] = 5;
  x[1] = 10;
  x[2] = 0.25;
  std::pair<Real, Vector> fg = problem->computeObjectiveAndGradient(x);

  delete problem;
  return 0;
}

// #include "Optimizer.hpp"
// #include <iostream>
//
// std::vector<std::pair<ADReal, ADReal>>
// readVector2(std::string filename)
// {
//   std::vector<std::pair<ADReal, ADReal>> x;
//   std::ifstream infile(filename);
//   std::string line;
//   while (std::getline(infile, line))
//   {
//     std::istringstream iss(line);
//     Real xi, xj;
//     if (!(iss >> xi >> xj))
//       break;
//     x.push_back({xi, xj});
//   }
//   infile.close();
//   return x;
// }
//
// std::vector<ADReal>
// readVector(std::string filename)
// {
//   std::vector<ADReal> x;
//   std::ifstream infile(filename);
//   std::string line;
//   while (std::getline(infile, line))
//   {
//     std::istringstream iss(line);
//     Real xi;
//     if (!(iss >> xi))
//       break;
//     x.push_back(xi);
//   }
//   infile.close();
//   return x;
// }
//
// std::array<Real, NVARS>
// readx(std::string filename)
// {
//   std::array<Real, NVARS> x;
//   std::ifstream infile(filename);
//   std::string line;
//   for (unsigned int i = 0; i < NVARS; i++)
//   {
//     std::getline(infile, line);
//     std::istringstream iss(line);
//     Real xi;
//     if (!(iss >> xi))
//       break;
//     x[i] = xi;
//   }
//   infile.close();
//   return x;
// }
//
// static char help[] = "Inverse identification";
//
// PetscErrorCode FormFunctionGradient(Tao, Vec, PetscReal *, Vec, void *);
// PetscErrorCode FormHessian(Tao, Vec, Mat, Mat, void *);
//
// int
// main(int argc, char ** argv)
// {
//   std::vector<std::pair<ADReal, ADReal>> lambda1s = readVector2(argv[1]);
//   std::vector<ADReal> sigma_exp = readVector(argv[2]);
//
//   std::array<Real, NVARS> empty;
//   Optimizer * problem = new Optimizer(empty, lambda1s, sigma_exp);
//
//   PetscErrorCode ierr;
//   PetscReal zero = 0;
//   PetscMPIInt size;
//   Tao tao;
//   Vec x, lb, ub;
//   Mat H;
//   PetscInt ind[] = {0, 1, 2, 3, 4, 5, 6};
//   PetscReal x0[] = {1, 1, 0, 10, 0, 10, 0.25};
//   PetscReal ub_vals[] = {PETSC_INFINITY,
//                          PETSC_INFINITY,
//                          PETSC_INFINITY,
//                          PETSC_INFINITY,
//                          PETSC_INFINITY,
//                          PETSC_INFINITY,
//                          1};
//   const PetscReal * solution;
//
//   ierr = PetscInitialize(&argc, &argv, (char *)0, help);
//   if (ierr)
//     return ierr;
//   ierr = MPI_Comm_size(PETSC_COMM_WORLD, &size);
//   if (size != 1)
//     SETERRQ(PETSC_COMM_WORLD, PETSC_ERR_WRONG_MPI_SIZE, "Incorrect number of processors");
//
//   ierr = VecCreateSeq(PETSC_COMM_WORLD, 7, &x);
//   ierr = MatCreateSeqAIJ(PETSC_COMM_WORLD, 7, 7, 7, NULL, &H);
//
//   ierr = TaoCreate(PETSC_COMM_WORLD, &tao);
//   ierr = TaoSetType(tao, TAOBNTL);
//
//   /* Check for TAO command line options */
//   ierr = PetscOptionsSetValue(NULL, "-tao_monitor", NULL);
//   ierr = TaoSetFromOptions(tao);
//
//   /* Set solution vec and an initial guess */
//   ierr = VecSetValues(x, 7, ind, x0, INSERT_VALUES);
//   ierr = TaoSetInitialVector(tao, x);
//
//   /* Set routines for function, gradient, hessian evaluation */
//   ierr = TaoSetObjectiveAndGradientRoutine(tao, FormFunctionGradient, problem);
//   ierr = TaoSetHessianRoutine(tao, H, H, FormHessian, problem);
//
//   /* Set bounds */
//   ierr = VecCreateSeq(PETSC_COMM_WORLD, 7, &lb);
//   ierr = VecCreateSeq(PETSC_COMM_WORLD, 7, &ub);
//   ierr = VecSet(lb, zero);
//   ierr = VecSetValues(ub, 7, ind, ub_vals, INSERT_VALUES);
//   ierr = TaoSetVariableBounds(tao, lb, ub);
//
//   /* SOLVE THE APPLICATION */
//   ierr = TaoSolve(tao);
//
//   /* Print solution */
//   ierr = TaoGetSolutionVector(tao, &x);
//   ierr = VecGetArrayRead(x, &solution);
//   std::cout << "mu4 = " << solution[3] << std::endl;
//   std::cout << "beta4 = " << solution[5] << std::endl;
//   std::cout << "rho = " << solution[6] << std::endl;
//   ierr = VecRestoreArrayRead(x, &solution);
//
//   /* Clean up */
//   ierr = TaoDestroy(&tao);
//   ierr = VecDestroy(&x);
//   ierr = MatDestroy(&H);
//
//   ierr = PetscFinalize();
//   delete problem;
//
//   return ierr;
// }
//
// PetscErrorCode
// FormFunctionGradient(Tao tao, Vec X, PetscReal * f, Vec G, void * ptr)
// {
//   Optimizer * problem = static_cast<Optimizer *>(ptr);
//   PetscErrorCode ierr;
//   PetscReal * g;
//   const PetscReal * x;
//
//   /* Get pointers to vector data */
//   ierr = VecGetArrayRead(X, &x);
//   ierr = VecGetArray(G, &g);
//
//   /* Compute f(X) */
//   ADReal objective = problem->objective(x);
//   *f = objective.value().value();
//
//   /* Compute G(X) */
//   Gradient gradient = problem->getGradient(objective, {0, 1, 2, 3, 4, 5, 6});
//   for (unsigned int i = 0; i < gradient.rows(); i++)
//     g[i] = gradient[i];
//
//   /* Restore vectors */
//   ierr = VecRestoreArrayRead(X, &x);
//   ierr = VecRestoreArray(G, &g);
//
//   return ierr;
// }
//
// PetscErrorCode
// FormHessian(Tao tao, Vec X, Mat H, Mat Hpre, void * ptr)
// {
//   Optimizer * problem = static_cast<Optimizer *>(ptr);
//   PetscErrorCode ierr;
//   PetscInt ind[] = {0, 1, 2, 3, 4, 5, 6};
//   PetscReal v[7][7];
//   const PetscReal * x;
//   PetscBool assembled;
//
//   /* Zero existing matrix entries */
//   ierr = MatAssembled(H, &assembled);
//   if (assembled)
//     ierr = MatZeroEntries(H);
//
//   /* Get a pointer to vector data */
//   ierr = VecGetArrayRead(X, &x);
//
//   /* Compute H(X) entries */
//   ADReal objective = problem->objective(x);
//   Hessian hessian = problem->getHessian(objective, {0, 1, 2, 3, 4, 5, 6});
//   for (unsigned int i = 0; i < hessian.rows(); i++)
//     for (unsigned int j = 0; j < hessian.rows(); j++)
//       v[i][j] = hessian(i, j);
//   ierr = MatSetValues(H, 7, ind, 7, ind, v[0], INSERT_VALUES);
//
//   ierr = VecRestoreArrayRead(X, &x);
//
//   /* Assemble matrix */
//   ierr = MatAssemblyBegin(H, MAT_FINAL_ASSEMBLY);
//   ierr = MatAssemblyEnd(H, MAT_FINAL_ASSEMBLY);
//
//   return ierr;
// }
