#--------------------------------------------------------
# The CMakeLists.txt for:                       pBearingAngle
# Author(s):                              CTHung
#--------------------------------------------------------

SET(SRC
  BearingAngle.cpp
  BearingAngle_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pBearingAngle ${SRC})

TARGET_LINK_LIBRARIES(pBearingAngle
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

