#include "Optimizer.hpp"

std::vector<ADReal>
readVector(std::string filename)
{
  std::vector<ADReal> x;
  std::ifstream infile(filename);
  std::string line;
  while (std::getline(infile, line))
  {
    std::istringstream iss(line);
    Real xi;
    if (!(iss >> xi))
      break;
    x.push_back(xi);
  }
  infile.close();
  return x;
}

std::array<Real, NVARS>
readx(std::string filename)
{
  std::array<Real, NVARS> x;
  std::ifstream infile(filename);
  std::string line;
  for (unsigned int i = 0; i < NVARS; i++)
  {
    std::getline(infile, line);
    std::istringstream iss(line);
    Real xi;
    if (!(iss >> xi))
      break;
    x[i] = xi;
  }
  infile.close();
  return x;
}

int
main(int argc, char const * argv[])
{
  std::vector<ADReal> lambda1s = readVector(argv[1]);
  std::vector<ADReal> sigma_exp = readVector(argv[2]);

  std::array<Real, NVARS> x = readx("x.csv");
  Optimizer o(x, lambda1s, sigma_exp);
  ADReal f = o.objective();
  Gradient g = o.getGradient(f, {0, 1, 2, 3});
  Hessian H = o.getHessian(f, {0, 1, 2, 3});

  std::ofstream function_file("f.csv");
  std::ofstream gradient_file("g.csv");
  std::ofstream Hessian_file("H.csv");

  function_file << f.value().value() << std::endl;
  gradient_file << g << std::endl;
  Hessian_file << H << std::endl;

  function_file.close();
  gradient_file.close();
  Hessian_file.close();
}
