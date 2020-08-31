# Install script for directory: C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/AdolcForward"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/AlignedVector3"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/ArpackSupport"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/AutoDiff"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/BVH"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/EulerAngles"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/FFT"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/IterativeSolvers"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/KroneckerProduct"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/LevenbergMarquardt"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/MatrixFunctions"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/MoreVectorization"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/MPRealSupport"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/NonLinearOptimization"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/NumericalDiff"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/OpenGLSupport"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/Polynomials"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/Skyline"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/SparseExtra"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/SpecialFunctions"
    "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/Splines"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Felipe/source/repos/FelipeSempertegui/FEM_renderer_middleEar/Dependencies/eigen-3.3.7/out/build/x64-Debug/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

