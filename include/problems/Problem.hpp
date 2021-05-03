#pragma once

#include "Types.hpp"
#include "Utils.hpp"

class Variable;
class Objective;
class Solver;
class UserObject;

class Problem
{
public:
  Problem();
  virtual ~Problem();

  void setUp(hit::Node * root);

  // @{ Dof related APIs
  const DofMap & dofMap() const { return _dof_map; }
  std::vector<DofId> dofs() const { return _dofs; }
  std::vector<Real> dofValues() const;
  void setDofValues(const std::vector<Real> & v);
  void setDofValues(const Real * v);
  // @}

  // @{ Variable related APIs
  Variable * variable(const VariableName name) const;
  unsigned int numVars() const { return _variables.size(); }
  void addVariable(hit::Node * params, const bool primary = true);
  void setVariableValue(const VariableName var, Real v);
  std::vector<Real> lowerBound() const;
  std::vector<Real> upperBound() const;
  // @}

  // @{ Objective related APIs
  Objective * objective() const { return _objective; }
  void addObjective(hit::Node * params);
  Vector gradient(const ADReal & v) const;
  Matrix Hessian(const ADReal & v) const;
  Real firstDerivative(const ADReal & v, const VariableName var1) const;
  Real secondDerivative(const ADReal & v, const VariableName var1, const VariableName var2) const;
  // @}

  // @{ Solver related APIs
  void addSolver(hit::Node * params);
  void solve();
  // @}

  // @{ UserObject related APIs
  UserObject * userObject(const UserObjectName name) const;
  void addUserObject(hit::Node * params);
  // @}

  unsigned int & indent() { return _indent; }

protected:
private:
  void initVariableDerivatives();

  DofMap _dof_map;
  std::vector<DofId> _dofs;

  std::vector<Variable *> _variables;
  std::vector<Variable *> _primary_variables;

  Objective * _objective;

  Solver * _solver;

  std::vector<UserObject *> _userobjects;

  unsigned int _indent;

  friend std::ostream & operator<<(std::ostream & os, Problem & p);
};

std::ostream & operator<<(std::ostream & os, const Problem & p);
