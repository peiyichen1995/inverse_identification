#pragma once

#include "Types.hpp"
#include "Utils.hpp"

class Variable;
class Objective;
class Solver;

class Problem
{
public:
  Problem();
  virtual ~Problem();

  void init();

  // @{ Dof related APIs
  std::vector<DofId> dofs() const { return _dofs; }
  std::vector<Real> dofValues() const;
  void setDofValues(const std::vector<Real> & v);
  void setDofValues(const Real * v);
  // @}

  // @{ Variable related APIs
  Variable * variable(const VariableName name) const;
  unsigned int numVars() const { return _variables.size(); }
  void addVariable(hit::Node * params, const bool primary = true);
  // @}

  // @{ Objective related APIs
  Objective * objective() const { return _objective; }
  void addObjective(hit::Node * params);
  Vector gradient(const ADReal & v) const;
  Matrix Hessian(const ADReal & v) const;
  // @}

  // @{ Solver related APIs
  void addSolver(hit::Node * params);
  void solve();
  // @}

protected:
private:
  void initVariableDerivatives();

  DofMap _dof_map;
  std::vector<DofId> _dofs;

  std::vector<Variable *> _variables;
  std::vector<Variable *> _primary_variables;

  Objective * _objective;

  Solver * _solver;

  friend std::ostream & operator<<(std::ostream & os, const Problem & p);
};

std::ostream & operator<<(std::ostream & os, const Problem & p);
