#include "Problem.hpp"
#include <cstdlib>

int
main(int argc, const char * argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: ./optimize input_file.i <cli_args...>" << std::endl;
    exit(1);
  }

  // Parse input file
  std::ifstream file(argv[1]);
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string input = buffer.str();
  std::unique_ptr<hit::Node> root(hit::parse("Hit parser", input));

  // Replace CLI args
  std::vector<std::string> cliargs(argv + 1, argv + argc);
  std::string cli_input = Utils::hitCLIFilter(cliargs);
  std::unique_ptr<hit::Node> cli_root(hit::parse("Hit cliargs", cli_input));
  hit::explode(cli_root.get());
  hit::explode(root.get());
  hit::merge(cli_root.get(), root.get());

  // Expand input file
  hit::BraceExpander expander("Hit expander");
  hit::EnvEvaler env;
  hit::RawEvaler raw;
  expander.registerEvaler("env", env);
  expander.registerEvaler("raw", raw);
  root->walk(&expander);

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

  // Add user objects
  std::vector<hit::Node *> uos = root->find("UserObjects")->children(hit::NodeType::Section);
  for (auto uo : uos)
    problem->addUserObject(uo);

  // Add objective
  hit::Node * obj = root->find("Objective");
  problem->addObjective(obj);

  // Add solver
  hit::Node * solver = root->find("Solver");
  problem->addSolver(solver);

  // Solve
  std::cout << *problem << std::endl;
  problem->solve();

  delete problem;

  return 0;
}
