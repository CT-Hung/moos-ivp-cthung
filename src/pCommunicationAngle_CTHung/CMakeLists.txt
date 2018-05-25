#--------------------------------------------------------
# The CMakeLists.txt for:                       pCommunicationAngle_CTHung
# Author(s):                              CT_Hung
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  CommunicationAngle_CTHung.cpp
  CommunicationAngle_CTHung_Info.cpp
  main.cpp
	soundTrack.cpp
)



ADD_EXECUTABLE(pCommunicationAngle_CTHung ${SRC})

TARGET_LINK_LIBRARIES(pCommunicationAngle_CTHung
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

