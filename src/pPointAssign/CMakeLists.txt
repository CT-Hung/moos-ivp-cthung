#--------------------------------------------------------
# The CMakeLists.txt for:                       pPointAssign
# Author(s):                              CTHung
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  PointAssign.cpp
  PointAssign_Info.cpp
  main.cpp
	AssignPoint.cpp
)



ADD_EXECUTABLE(pPointAssign ${SRC})

TARGET_LINK_LIBRARIES(pPointAssign
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

