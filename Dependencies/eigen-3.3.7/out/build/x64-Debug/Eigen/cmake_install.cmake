# Install script for directory: D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/out/install/x64-Debug")
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
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/Cholesky"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/CholmodSupport"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/Core"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/Dense"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/Eigen"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/Eigenvalues"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/Geometry"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/Householder"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/IterativeLinearSolvers"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/Jacobi"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/LU"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/MetisSupport"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/OrderingMethods"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/PaStiXSupport"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/PardisoSupport"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/QR"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/QtAlignedMalloc"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/SPQRSupport"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/SVD"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/Sparse"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/SparseCholesky"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/SparseCore"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/SparseLU"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/SparseQR"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/StdDeque"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/StdList"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/StdVector"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/SuperLUSupport"
    "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/UmfPackSupport"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE DIRECTORY FILES "D:/Documents/learn C++/passo_17_final/Project1/Dependencies/eigen-3.3.7/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

