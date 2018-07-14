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
   void play();


 private: // Configuration variables
    std::string m_sh_file;
    std::string m_start_play;

    double m_times;
    double m_play_times;
    double m_play_duration;
    double m_start_time;
    double m_end_time;

    bool m_flag_duration;
    bool m_play_inf;

    int m_total_play;

 private: // State variables
};

#endif 
