# Install script for directory: /Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/OpenGL/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/AllBulletDemos/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/ConvexDecompositionDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/Benchmarks/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/HelloWorld/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/MultiThreadedDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/CcdPhysicsDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/ConstraintDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/SliderConstraintDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/GenericJointDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/RagdollDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/ForkLiftDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/BasicDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/BspDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/MovingConcaveDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/VehicleDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/ColladaDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/UserCollisionAlgorithm/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/CharacterDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/SoftDemo/cmake_install.cmake")
  INCLUDE("/Users/jomanto/coding/wednesnight/branches/lua-test-branch/thirdparty/bullet/bullet_2_74/Demos/HeightFieldFluidDemo/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

