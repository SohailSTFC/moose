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

#pragma once

#include "ProblemOperator.h"
#include "EquationSystemInterface.h"
#include "dfemEquationSystem.h"

// Dual number
using mfem::future::dual;
using dual_t = dual<mfem::real_t, mfem::real_t>;

namespace Moose::MFEM
{
/// Steady-state problem operator with an dfem equation system.
class dfemEquationSystemProblemOperator : public ProblemOperator, public EquationSystemInterface
{
public:
  dfemEquationSystemProblemOperator(MFEMProblem & problem)
    : ProblemOperator(problem),
      _equation_system(std::dynamic_pointer_cast<dfemEquationSystem<dual_t,2>>(_problem_data.eqn_system))
  {
  }

  void SetGridFunctions() override;
  void Init(mfem::BlockVector & X) override;
  virtual void Solve() override;

  [[nodiscard]] Moose::MFEM::dfemEquationSystem<dual_t,2> * GetEquationSystem() const override
  {
    mooseAssert(_equation_system, "No EquationSystem in dfemEquationSystemProblemOperator.");
    return _equation_system.get();
  }

private:
  // Default to dual numbers for now
  // later on add support for enzyme
  // supported for 2-D
  std::shared_ptr<Moose::MFEM::dfemEquationSystem<dual_t,2> > _equation_system{nullptr};
};

} // namespace Moose::MFEM

#endif
