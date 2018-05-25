#--------------------------------------------------------
# The CMakeLists.txt for:                       pGenPath
# Author(s):                              CTHung
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  GenPath.cpp
  GenPath_Info.cpp
  main.cpp
	GetPosition.cpp
)



ADD_EXECUTABLE(pGenPath ${SRC})

TARGET_LINK_LIBRARIES(pGenPath
   ${MOOS_LIBRARIES}
   geometry
	 mbutil
   m
   pthread
	 )

