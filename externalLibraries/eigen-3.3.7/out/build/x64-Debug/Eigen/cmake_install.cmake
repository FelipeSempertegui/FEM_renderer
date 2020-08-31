# Install script for directory: C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/Felipe/Source/Repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE FILE FILES
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/Cholesky"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/CholmodSupport"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/Core"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/Dense"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/Eigen"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/Eigenvalues"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/Geometry"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/Householder"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/IterativeLinearSolvers"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/Jacobi"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/LU"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/MetisSupport"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/OrderingMethods"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/PaStiXSupport"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/PardisoSupport"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/QR"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/QtAlignedMalloc"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/SPQRSupport"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/SVD"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/Sparse"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/SparseCholesky"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/SparseCore"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/SparseLU"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/SparseQR"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/StdDeque"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/StdList"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/StdVector"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/SuperLUSupport"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/UmfPackSupport"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE DIRECTORY FILES "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

