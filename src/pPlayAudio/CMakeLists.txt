#--------------------------------------------------------
# The CMakeLists.txt for:                       pPlayAudio
# Author(s):                              CTHung
#--------------------------------------------------------

SET(SRC
  PlayAudio.cpp
  PlayAudio_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pPlayAudio ${SRC})

TARGET_LINK_LIBRARIES(pPlayAudio
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread
   asound)

