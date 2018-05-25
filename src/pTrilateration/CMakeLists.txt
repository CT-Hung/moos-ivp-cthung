#--------------------------------------------------------
# The CMakeLists.txt for:                       pTrilateration
# Author(s):                              cthung
#--------------------------------------------------------

SET(SRC
  Trilateration.cpp
	Navigation.cpp
  Trilateration_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pTrilateration ${SRC})

TARGET_LINK_LIBRARIES(pTrilateration
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

