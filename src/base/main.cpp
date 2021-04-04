#include "Problem.hpp"
#include <petsctao.h>
#include <cstdlib>

int
main(int argc, const char * argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: ./optimize input_file.i" << std::endl;
    exit(1);
  }

  // Parse input file
  std::ifstream file(argv[1]);
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string input = buffer.str();
  std::unique_ptr<hit::Node> root(hit::parse("Hit", input));

  // Create the optimization problem
  auto problem = new Problem();

  // Add variables
  std::vector<hit::Node *> vars = root->find("Variables")->children(hit::NodeType::Section);
  for (auto var : vars)
    problem->addVariable(var, true);

  // Add auxiliary variables
  std::vector<hit::Node *> aux_vars = root->find("AuxVariables")->children(hit::NodeType::Section);
  for (auto aux_var : aux_vars)
    problem->addVariable(aux_var, false);

  // Add objective
  hit::Node * obj = root->find("Objective");
  problem->addObjective(obj);

  // Add solver
  hit::Node * solver = root->find("Solver");
  problem->addSolver(solver);

  // Initialize
  problem->init();
  std::cout << *problem << std::endl;

  problem->solve();

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

// PetscErrorCode FormFunctionGradient(Tao, Vec, PetscReal *, Vec, void *);
// PetscErrorCode FormHessian(Tao, Vec, Mat, Mat, void *);

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
