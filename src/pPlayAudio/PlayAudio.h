/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PlayAudio.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef PlayAudio_HEADER
#define PlayAudio_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <alsa/asoundlib.h>

class PlayAudio : public AppCastingMOOSApp
{
 public:
   PlayAudio();
   ~PlayAudio();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();


 private: // Configuration variables
    std::string m_sh_file;
    std::string m_start_play;
    std::string m_play_inf;
    double m_times;
    double m_play_times;
 private: // State variables
};

#endif 
