//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
//**/
#ifdef MOOSE_MFEM_ENABLED

#pragma once

#include "MFEMGeneralUserObject.h"
#include "MFEMContainers.h"
#include "MFEMBlockRestrictable.h"

/**
 * Class to construct an MFEM dFEM integrator to apply to the dfem equation system.
 */
class MFEMdFemKernel : public MFEMGeneralUserObject, public MFEMBlockRestrictable
{
public:
  static InputParameters validParams();

  MFEMdFemKernel(const InputParameters & parameters);
  virtual ~MFEMdFemKernel() = default;

  /// Create MFEM integrator to apply to the LHS of the weak form. Ownership managed by the caller.
  virtual mfem::Operator * createResidual() { return nullptr; }
  virtual mfem::Operator * createJacobianI(const int& i [[maybe_unused]]) { return nullptr; }

  /// Get name of the test variable labelling the weak form this kernel is added to
  const VariableName & getTestVariableName() const { return _test_var_name; }

  /// Get name of the trial variable (gridfunction) the kernel acts on.
  /// No default as these kernels have an arbitrary sized range
  virtual const int & getNumberOfTrialVars() const {return numTrialVarsExpected;}
  virtual const bool & isTrialSpaceActive(const int& i) const {return _trial_var_names_active[i].second;}
  virtual const VariableName & getTrialVariableName(const int& i) const { return _trial_var_names_active[i].first;}

protected:
  /// Name of (the test variable associated with) the weak form that the kernel is applied to.
  const int numTrialVarsExpected;
  const VariableName& _test_var_name;

  /// Mapping of trial vars names to gridfunctions and a boolean to check whether
  /// the Jacobian contribution of the trial variable is added/active (the residual
  /// of the contribution is always added)
  std::vector<std::pair<const VariableName&,bool>> _trial_var_names_active;
};

#endif
