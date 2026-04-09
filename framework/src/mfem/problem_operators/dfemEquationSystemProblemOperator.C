//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

//*//
#ifdef MOOSE_MFEM_ENABLED

#include "dfemEquationSystemProblemOperator.h"

namespace Moose::MFEM
{
void
dfemEquationSystemProblemOperator::SetGridFunctions()
{
  _trial_var_names = GetEquationSystem()->GetTrialVarNames();
  _test_var_names = GetEquationSystem()->GetTestVarNames();
  ProblemOperator::SetGridFunctions();
}

void
dfemEquationSystemProblemOperator::Init(mfem::BlockVector & X)
{
  ProblemOperator::Init(X);
//  GetEquationSystem()->BuildEquationSystem();
  // Assign initial condition as initial guess for non-linear problems
//  for (const auto i : index_range(_trial_variables))
//    *(GetEquationSystem()->_var_ess_constraints.at(i)) = *_trial_variables[i];
}

void
dfemEquationSystemProblemOperator::Solve()
{
  _problem_data.nonlinear_solver->SetOperator(*GetEquationSystem());
  _problem_data.nonlinear_solver->Mult(_true_rhs, _true_x);
  GetEquationSystem()->SetTrialVariablesFromTrueVectors(_true_x);
}

} // namespace Moose::MFEM

#endif
