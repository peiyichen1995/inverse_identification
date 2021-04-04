#include "Problem.hpp"
#include "Variable.hpp"
#include "Objective.hpp"
#include "Solver.hpp"

Problem::Problem() : _objective(nullptr), _solver(nullptr) {}

Problem::~Problem()
{
  delete _objective;
  for (auto v : _variables)
    delete v;
}

void
Problem::init()
{
  initVariableDerivatives();
}

void
Problem::initVariableDerivatives()
{
  unsigned int n = numVars();
  for (auto var : _variables)
  {
    ADReal & v = var->set();
    DofId dof = _dof_map[var->name()];
    for (unsigned int i = 0; i < n; i++)
    {
      DofId dof_i = _dof_map[_variables[i]->name()];
      v.value().derivatives().insert(dof_i) = dof == dof_i ? 1 : 0;
      v.derivatives().insert(dof_i).value() = dof == dof_i ? 1 : 0;
      for (unsigned int j = 0; j < n; j++)
      {
        DofId dof_j = _dof_map[_variables[j]->name()];
        v.derivatives().insert(dof_i).derivatives().insert(dof_j) = 0;
      }
    }
  }
}

std::vector<Real>
Problem::dofValues() const
{
  std::vector<Real> v(_dofs.size());
  for (auto var : _primary_variables)
    v[_dof_map.at(var->name())] = raw_value(var->value());
  return v;
}

void
Problem::setDofValues(const std::vector<Real> & v)
{
  for (auto var : _primary_variables)
    var->set().value().value() = v[_dof_map[var->name()]];
}

void
Problem::setDofValues(const Real * v)
{
  for (auto var : _primary_variables)
    var->set().value().value() = v[_dof_map[var->name()]];
}

Variable *
Problem::variable(const VariableName name) const
{
  for (auto v : _variables)
    if (v->name() == name)
      return v;

  std::cout << "Trying to get variable " << name << " that doesn't exist in the problem."
            << std::endl;
  exit(1);
}

void
Problem::addVariable(hit::Node * params, const bool primary)
{
  for (auto v : _variables)
    if (v->name() == params->path())
    {
      std::cout << "Trying to add a variable " << v->name()
                << " that already exists in the problem." << std::endl;
      exit(1);
    }
  auto v = new Variable(params);
  _variables.push_back(v);

  DofId dof_id = _dof_map.size();
  _dof_map.emplace(params->path(), dof_id);
  if (primary)
  {
    _primary_variables.push_back(v);
    _dofs.push_back(dof_id);
  }
}

void
Problem::addObjective(hit::Node * params)
{
  if (_objective)
  {
    std::cout << "Trying to add an objective but one already exists." << std::endl;
    exit(1);
  }
  _objective = static_cast<Objective *>(
      Factory::createObject(params->param<std::string>("type"), this, params));
}

Vector
Problem::gradient(const ADReal & v) const
{
  Vector g(_dofs.size());
  for (auto i : _dofs)
    g[i] = v.value().derivatives()[i];
  return g;
}

Matrix
Problem::Hessian(const ADReal & v) const
{
  Matrix H(_dofs.size(), _dofs.size());
  for (auto i : _dofs)
    for (auto j : _dofs)
      H(i, j) = v.derivatives()[i].derivatives()[j];
  return H;
}

void
Problem::addSolver(hit::Node * params)
{
  if (_solver)
  {
    std::cout << "Trying to add a solver but one already exists." << std::endl;
    exit(1);
  }
  _solver = static_cast<Solver *>(
      Factory::createObject(params->param<std::string>("type"), this, params));
}

void
Problem::solve()
{
  _solver->solve();
}

std::ostream &
operator<<(std::ostream & os, const Problem & p)
{
  os << Utils::dline << std::endl;
  os << "Problem definition" << std::endl;

  os << Utils::sline << std::endl;

  os << Utils::indent(1) << "Variables:" << std::endl;
  for (auto v : p._variables)
    os << Utils::indent(2) << *v << std::endl;

  os << Utils::sline << std::endl;

  os << Utils::indent(1) << "Dof map: " << std::endl;
  for (auto nd : p._dof_map)
    os << Utils::indent(2) << nd.first << ": " << nd.second << std::endl;

  os << Utils::sline << std::endl;

  os << Utils::indent(1) << "Primary dofs: " << std::endl;
  os << Utils::indent(2);
  for (auto dof : p._dofs)
    os << dof << " ";
  os << std::endl;

  os << Utils::sline << std::endl;

  os << Utils::indent(1) << "Objective: " << std::endl;
  os << Utils::indent(2) << p._objective->type() << std::endl;

  os << Utils::sline << std::endl;

  os << Utils::indent(1) << "Solver: " << std::endl;
  os << Utils::indent(2) << p._solver->type() << std::endl;

  os << Utils::dline << std::endl;

  return os;
}
