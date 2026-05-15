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
  [HCurlFESpace]
    type = MFEMVectorFESpace
    fec_type = ND
    fec_order = SECOND
  []
[]

[Variables]
   [e_field]
    type = MFEMVariable
    fespace = HCurlFESpace
  []
[]

[Functions]
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

  [crap_e_field]
    type = ParsedVectorFunction
    expression_x = 'sin(kappa * y)'
    expression_y = 'sin(kappa * z)'
    expression_z = 'sin(kappa * x)'

    symbol_names = kappa
    symbol_values = 3.1415926535
  []
[]

[ICs]
  [crap_e_field_ic]
    type = MFEMVectorIC
    vector_coefficient = crap_e_field
    variable = e_field
  []
[]

[BCs]
  [tangential_E_bdr]
    type = MFEMVectorTangentialDirichletBC
    variable = e_field
    boundary = 'Coil'
    vector_coefficient = crap_e_field
  []
[]

[Kernels]
  [nl_curlcurl]
    type = MFEMNLCurlCurlKernel
    variable = e_field
    k_coefficient = kFunc
    dk_dcu_coefficient = dkFunc_dB
  []
  [mass]
    type = MFEMVectorFEMassKernel
    variable = e_field
    coefficient = 1.0e-1
  []
[]

[Solver]
   type = MFEMMUMPS
   print_level = 0
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
