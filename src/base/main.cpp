#include "Problem.hpp"
#include "InputParser.hpp"

int
main(int argc, const char * argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: ./optimize input_file.i <cli_args...>" << std::endl;
    exit(1);
  }

  // Parse input file
  InputParser input(argc, argv);

  // Create the optimization problem
  auto problem = new Problem();
  problem->setUp(input.root());
  std::cout << *problem << std::endl;

  // Solve
  problem->solve();

  // Clean up
  delete problem;

  return 0;
}
