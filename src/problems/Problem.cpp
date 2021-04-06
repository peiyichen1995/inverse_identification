#include "Problem.hpp"
#include "Variable.hpp"
#include "Objective.hpp"
#include "Solver.hpp"
#include "UserObject.hpp"

Problem::Problem() : _objective(nullptr), _solver(nullptr) {}

Problem::~Problem()
{
  for (auto v : _variables)
    delete v;

  delete _objective;

  delete _solver;

  for (auto uo : _userobjects)
    delete uo;
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
Problem::setVariableValue(const VariableName var, Real v)
{
  variable(var)->set().value().value() = v;
}

std::vector<Real>
Problem::lowerBound() const
{
  std::vector<Real> lb(_primary_variables.size());
  for (auto v : _primary_variables)
  {
    DofId dof = _dof_map.at(v->name());
    lb[dof] = v->lowerBound();
  }
  return lb;
}

std::vector<Real>
Problem::upperBound() const
{
  std::vector<Real> ub(_primary_variables.size());
  for (auto v : _primary_variables)
  {
    DofId dof = _dof_map.at(v->name());
    ub[dof] = v->upperBound();
  }
  return ub;
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

Real
Problem::firstDerivative(const ADReal & v, const VariableName var1) const
{
  DofId i = _dof_map.at(var1);
  return v.value().derivatives()[i];
}

Real
Problem::secondDerivative(const ADReal & v, const VariableName var1, const VariableName var2) const
{
  DofId i = _dof_map.at(var1);
  DofId j = _dof_map.at(var2);
  return v.derivatives()[i].derivatives()[j];
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
  initVariableDerivatives();
  _solver->solve();

  std::cout << "\n" << Utils::dline << std::endl;
  std::cout << "Solution:" << std::endl;
  for (auto v : _primary_variables)
    std::cout << Utils::indent(1) << v->name() << " = " << raw_value(v->value()) << std::endl;
  std::cout << Utils::dline << std::endl;
}

UserObject *
Problem::userObject(const UserObjectName name) const
{
  for (auto uo : _userobjects)
    if (uo->name() == name)
      return uo;

  std::cout << "Trying to get user object " << name << " that doesn't exist in the problem."
            << std::endl;
  exit(1);
}

void
Problem::addUserObject(hit::Node * params)
{
  for (auto uo : _userobjects)
    if (uo->name() == params->path())
    {
      std::cout << "Trying to add a user object " << uo->name()
                << " that already exists in the problem." << std::endl;
      exit(1);
    }
  UserObject * uo = static_cast<UserObject *>(
      Factory::createObject(params->param<std::string>("type"), this, params));
  _userobjects.push_back(uo);
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

  os << Utils::sline << std::endl;

  os << Utils::indent(1) << "UserObjects: " << std::endl;
  for (auto uo : p._userobjects)
    os << Utils::indent(2) << uo->name() << std::endl;

  os << Utils::dline << std::endl;

  return os;
}
