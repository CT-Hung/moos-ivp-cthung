#--------------------------------------------------------
# The CMakeLists.txt for:                       pCheckSound
# Author(s):                              CTHung
#--------------------------------------------------------

SET(SRC
  CheckSound.cpp
  CheckSound_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pCheckSound ${SRC})

TARGET_LINK_LIBRARIES(pCheckSound
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread
   asound)

