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
 * Class to construct an MFEM dFEM integrator kernel to apply to the dfem equation system.
 */
template<typename dscalar_t, int dim>
class MFEMdfemKernel : public MFEMGeneralUserObject, public MFEMBlockRestrictable
{
  public:

  /// Get name of the test variable labelling the weak form this kernel is added to
  const VariableName & getTestVariableName() const { return _test_var_name; }

  /// Get name of the trial variable (gridfunction) the kernel acts on.
  /// No default as these kernels have an arbitrary sized range
  virtual const int & getNumberOfTrialVars() const {return numTrialSpacesExpected;}
  virtual const bool & isTrialSpaceActive(const int& i) const {return _trial_var_names_active[i].second;}
  virtual const VariableName & getTrialVariableName(const int& i) const { return _trial_var_names_active[i].first;}

  /// Add a residual contribution
  virtual void AddResidualIntegrator(mfem::future::DifferentiableOperator& Res);

  /// Add a Jacobian contribution
  virtual void AddJacobianIntegrator(mfem::future::DifferentiableOperator& Jac
                                   , const VariableName& trialVar);

  protected:
    /// Name of the test and trial spaces
    const int numTrialSpacesExpected;
    VariableName& _test_var_name;
    std::vector<std::pair<const VariableName&,bool>> _trial_var_names_active;
};
#endif
