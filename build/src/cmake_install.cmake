# Install script for directory: /home/ual/moos-ivp-cthung/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "None")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/ual/moos-ivp-cthung/build/src/lib_behaviors-test/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/lib_behaviors-cthung/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pXRelayTest/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pExampleApp/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pOdometry/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pPrimeFactor/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pPrimeFactorTester/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pCommunicationAngle_CTHung/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pPointAssign/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pGenPath/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pHazardPath/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/uFldHazardMgrX/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/uFldHazardMgrC/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pTrilateration/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pCheckSound/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pStoreSound/cmake_install.cmake")
  include("/home/ual/moos-ivp-cthung/build/src/pPlayAudio/cmake_install.cmake")

endif()

