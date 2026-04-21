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
#include "libmesh/ignore_warnings.h"
#include "mfem/miniapps/common/pfem_extras.hpp"
#include "libmesh/restore_warnings.h"
#include "MFEMIntegratedBC.h"
#include "MFEMEssentialBC.h"
#include "MFEMContainers.h"
#include "MFEMdfemKernel.h"

namespace Moose::MFEM
{

/**
 * Class to store weak form components (bilinear and linear forms, and optionally
 * mixed and nonlinear forms) and build methods
 */
template<typename dscalar_t, int dim = 2>
class dfemEquationSystem : public EquationSystem
{

private:

  // dFem kernels map, because they have variable numbers of trial
  // spaces dpenedant on the internal dFem kernal map
  NamedFieldsMap<std::shared_ptr<MFEMdfemKernel<dscalar_t,dim> >> _dfem_kernels;

  // Residual and Jacobian maps of the kernels
  NamedFieldsMap<mfem::Array<std::shared_ptr<mfem::Operator>>> _dfem_residuals;
  NamedFieldsMap<NamedFieldsMap<mfem::Array<std::shared_ptr<mfem::Operator>>>> _dfem_jacobians;

  /// Arrays to store essential BCs to act on each component of weak form.
  /// Named according to test variable.
  NamedFieldsMap<std::vector<std::shared_ptr<MFEMEssentialBC>>> _essential_bc_map;

  /// Friend classes
  friend class dfemEquationSystemProblemOperator;
  friend class ::MFEMProblemSolve;
  /// Disallowed inherited method
  /// using mfem::Operator::RecoverFEMSolution;

public:
  dfemEquationSystem() = default;
  ~dfemEquationSystem() override;

  /// Add BC associated with essentially constraint DoFs on boundaries.
  void AddEssentialBC(std::shared_ptr<MFEMEssentialBC> bc);

  /// Add a dfem domain integrator
  void AddDfemDomainKernel(std::shared_ptr<MFEMdfemKernel<dscalar_t,dim> > kernel);

  /// Apply the Residual forms
  void ApplyDomainResidualIntegrators(
      const std::string & test_var_name,
      mfem::Array<std::shared_ptr<mfem::Operator>> form,
      NamedFieldsMap<std::shared_ptr<MFEMdfemKernel<dscalar_t,dim> >> _dfem_kernels);

  /// Apply the Jacobian forms
  void ApplyDomainJacobianIntegrators(
      const std::string & test_var_name,
      NamedFieldsMap<mfem::Array<std::shared_ptr<mfem::Operator>>> form,
      NamedFieldsMap<std::shared_ptr<MFEMdfemKernel<dscalar_t,dim> >> _dfem_kernels);

}; //End of classname
} //End of namespace
#endif
