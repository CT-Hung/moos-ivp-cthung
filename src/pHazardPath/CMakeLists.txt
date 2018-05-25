#--------------------------------------------------------
# The CMakeLists.txt for:                       pHazardPath
# Author(s):                              Levi Cai
#--------------------------------------------------------

SET(SRC
  HazardPath.cpp
  HazardPath_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pHazardPathC ${SRC})

TARGET_LINK_LIBRARIES(pHazardPathC
   ${MOOS_LIBRARIES}
   geometry
   mbutil
   m
   pthread)

