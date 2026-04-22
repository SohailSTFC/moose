//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

//*/
#ifdef MOOSE_MFEM_ENABLED

#include "dfemEquationSystem.h"
#include "libmesh/int_range.h"

namespace Moose::MFEM
{

template<typename dscalar_t, int dim>
void
dfemEquationSystem<dscalar_t,dim>::AddEssentialBC(std::shared_ptr<MFEMEssentialBC> bc)
{
  const auto & test_var_name = bc->getTestVariableName();
  AddTestVariableNameIfMissing(test_var_name);
  // Register new essential bc map if not present for the test variable
  if (!_essential_bc_map.Has(test_var_name))
  {
    auto bcs = std::make_shared<std::vector<std::shared_ptr<MFEMEssentialBC>>>();
    _essential_bc_map.Register(test_var_name, std::move(bcs));
  }
  _essential_bc_map.GetRef(test_var_name).push_back(std::move(bc));
};


template<typename dscalar_t,int dim>
void
dfemEquationSystem<dscalar_t,dim>::AddDfemDomainKernel(std::shared_ptr<MFEMdfemKernel<dscalar_t,dim>> kernel)
{
  const auto & test_var_name = kernel.getTestVariableName();
  if( !_dfem_kernels.Has(test_var_name) )
  {
    auto kernelVector = std::make_shared<std::vector<std::shared_ptr<MFEMdfemKernel<dscalar_t,dim>>>>();
    _dfem_kernels.Register(test_var_name,std::move(kernelVector) );
  }
  _dfem_kernels.GetRef(test_var_name)->push_back(kernel);
}


template<typename dscalar_t,int dim>
void
dfemEquationSystem<dscalar_t,dim>::ApplyDomainResidualIntegrators(
      const std::string & test_var_name,
      std::shared_ptr<mfem::future::DifferentiableOperator> resForm,
      NamedFieldsMap<std::vector<std::shared_ptr<MFEMdfemKernel<dscalar_t,dim> >>> _dfem_kernels)
{
  if(_dfem_kernels.Has(test_var_name) )
  {
    auto kernels = _dfem_kernels.GetRef(test_var_name);
    for(auto & kernel : kernels)
    {
      kernel->AddResidualIntegrator(resForm);
    }
  }
};


template<typename dscalar_t,int dim>
void
dfemEquationSystem<dscalar_t,dim>::ApplyDomainJacobianIntegrators(
      const std::string & test_var_name,
      NamedFieldsMap<std::shared_ptr<mfem::future::DifferentiableOperator>> jacForm,
      NamedFieldsMap<std::vector<std::shared_ptr<MFEMdfemKernel<dscalar_t,dim> >>> _dfem_kernels)
{
  if(_dfem_kernels.Has(test_var_name) )
  {
    auto kernels = _dfem_kernels.GetRef(test_var_name);
    for(auto & kernel : kernels)
    {
      for(int i=0; i<kernel->getNumberOfTrialVars(); i++)
      {
        const auto trial_var_name = kernel->getTrialVariableName(i);
        if( kernel->isTrialSpaceActive(i) && jacForm.Has(trial_var_name) )
        {
          kernel->AddJacobianIntegrator(jacForm.GetRef(trial_var_name), trial_var_name);
        }
      }
    }
  }
};


template<typename dscalar_t,int dim>
void
dfemEquationSystem<dscalar_t,dim>::Mult(const mfem::Vector& x, mfem::Vector& y) const
{
};

template<typename dscalar_t,int dim>
mfem::Operator&
dfemEquationSystem<dscalar_t,dim>::GetGradient(const mfem::Vector& x) const
{
  return *_jacobian;
};


} // namespace Moose::MFEM

#endif
