##############################################################################
# FILE:        moos-ivp-extend/src/CMakeLists.txt
# DATE:        2010/09/07
# DESCRIPTION: CMakeLists.txt file for the moos-ivp-extend source directory
##############################################################################


#============================================================================
# Add the libraries in the current directory to the include path
#============================================================================
FILE(GLOB LOCAL_LIBRARY_DIRS ./lib_*)
INCLUDE_DIRECTORIES(${LOCAL_LIBRARY_DIRS})

#============================================================================
# List the subdirectories to build...
#============================================================================
ADD_SUBDIRECTORY(lib_behaviors-test)
ADD_SUBDIRECTORY(lib_behaviors-cthung)
ADD_SUBDIRECTORY(pXRelayTest)
ADD_SUBDIRECTORY(pExampleApp)
ADD_SUBDIRECTORY(pOdometry)
ADD_SUBDIRECTORY(pPrimeFactor)
ADD_SUBDIRECTORY(pPrimeFactorTester)
ADD_SUBDIRECTORY(pCommunicationAngle_CTHung)
ADD_SUBDIRECTORY(pPointAssign)
ADD_SUBDIRECTORY(pGenPath)
ADD_SUBDIRECTORY(pHazardPath)
ADD_SUBDIRECTORY(uFldHazardMgrX)
ADD_SUBDIRECTORY(uFldHazardMgrC)
ADD_SUBDIRECTORY(pTrilateration)
ADD_SUBDIRECTORY(pCheckSound)
ADD_SUBDIRECTORY(pStoreSound)
ADD_SUBDIRECTORY(pPlayAudio)
##############################################################################
#                           END of CMakeLists.txt
##############################################################################

