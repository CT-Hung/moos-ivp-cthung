#--------------------------------------------------------
# The CMakeLists.txt for:                       uFldHazardMgr
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  HazardMgr.cpp
  HazardMgr_Info.cpp
  main.cpp
	post2Vehicle.cpp
	post2Shore.cpp
)

ADD_EXECUTABLE(uFldHazardMgrC ${SRC})

TARGET_LINK_LIBRARIES(uFldHazardMgrC
   ${MOOS_LIBRARIES}
   ufield
   ufld_hazards
   geometry
   apputil
   mbutil
   ${SYSTEM_LIBS}
)

