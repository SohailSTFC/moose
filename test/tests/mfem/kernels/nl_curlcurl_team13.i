# TEAM 13 electromagnetics problem
# using a curl-curl formulation

[Mesh]
  type = MFEMMesh
  file = ../mesh/team_13.msh
  dim = 3
[]

[Problem]
  type = MFEMProblem
[]

[FESpaces]
  inactive = "L2FESpace"
  [HCurlFESpace]
    type = MFEMVectorFESpace
    fec_type = ND
    fec_order = FIRST
  []
  [HDivFESpace]
    type = MFEMVectorFESpace
    fec_type = RT
    fec_order = CONSTANT
  []
  [L2FESpace]
    type = MFEMScalarFESpace
    fec_type = L2
    fec_order = CONSTANT
  []
[]

[Variables]
  [e_field]
    type = MFEMVariable
    fespace = HCurlFESpace
  []
[]

[AuxVariables]
  inactive = "joule_heating"
  [db_dt_field]
    type = MFEMVariable
    fespace = HDivFESpace
  []
  [joule_heating]
    type = MFEMVariable
    fespace = L2FESpace
  []
[]

[AuxKernels]
  inactive = "joule_Q_aux"
  [curl]
    type = MFEMCurlAux
    variable = db_dt_field
    source = e_field
    scale_factor = -1.0
    execute_on = TIMESTEP_END
  []
  [joule_Q_aux]
    type = MFEMInnerProductAux
    variable = joule_heating
    first_source_vec = e_field
    second_source_vec = e_field
    execute_on = TIMESTEP_END
  []
[]

[Functions]
  [exact_e_field]
    type = ParsedVectorFunction
    expression_x = 'sin(kappa * y)'
    expression_y = 'sin(kappa * z)'
    expression_z = 'sin(kappa * x)'

    symbol_names = kappa
    symbol_values = 3.1415926535
  []

  [forcing_field]
    type = ParsedVectorFunction
    expression_x = '(1. + kappa * kappa) * sin(kappa * y)'
    expression_y = '(1. + kappa * kappa) * sin(kappa * z)'
    expression_z = '(1. + kappa * kappa) * sin(kappa * x)'

    symbol_names = kappa
    symbol_values = 3.1415926535
  []

  [epsilon]
    type = Constant
    expression = '(1. + kappa * kappa) * sin(kappa * y)'

    symbol_names = kappa
    symbol_values = 3.1415926535
  []

  [kFunc]
    type = ParsedFunction
    expression = '(1. + kappa * kappa) * sin(kappa * y)'

    symbol_names = kappa
    symbol_values = 3.1415926535
  []

  [dkFunc_dB]
    type = ParsedVectorFunction
    expression_x = '(1. + kappa * kappa) * sin(kappa * y)'
    expression_y = '(1. + kappa * kappa) * sin(kappa * z)'
    expression_z = '(1. + kappa * kappa) * sin(kappa * x)'

    symbol_names = kappa
    symbol_values = 3.1415926535
  []
[]

[BCs]
  [tangential_E_bdr]
    type = MFEMVectorTangentialDirichletBC
    variable = e_field
    vector_coefficient = exact_e_field
  []
[]

[Kernels]
  [nl_curlcurl]
    type = MFEMCurlCurlKernel
    variable = e_field
  []
  [mass]
    type = MFEMVectorFEMassKernel
    variable = e_field
    coefficient = 1.0e-1
  []
  [source]
    type = MFEMVectorFEDomainLFKernel
    variable = e_field
    vector_coefficient = forcing_field
  []
[]

[Preconditioner]
  [ams]
    type = MFEMHypreAMS
    fespace = HCurlFESpace
  []
[]

[Solver]
  type = MFEMHypreGMRES
  preconditioner = ams
  l_tol = 1e-12
[]

[Executioner]
  type = MFEMSteady
  device = cpu
  assembly_level = legacy
  nl_max_its = 30
  nl_abs_tol = 1.0e-5
  nl_rel_tol = 1.0e-5
  print_level = 1
[]

[Outputs]
  [ParaViewDataCollection]
    type = MFEMParaViewDataCollection
    file_base = OutputData/nl_CurlCurl
    vtk_format = ASCII
  []
[]       
