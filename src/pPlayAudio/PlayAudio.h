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

 protected:
   void setPlayParams();
   void PlayBack();

 private: // Configuration variables
   std::string m_playBackDevice;
   snd_pcm_t *m_handle;
   snd_pcm_hw_params_t *m_params;
   snd_pcm_uframes_t m_frames;
   unsigned int m_sampleRate;
   unsigned int m_channels;
   unsigned int m_playTime;
   unsigned int m_loops;
   unsigned int m_period_size;
   int m_dir;
   int m_bits;
   char *m_period_buffer;

 private: // State variables
};

#endif 
