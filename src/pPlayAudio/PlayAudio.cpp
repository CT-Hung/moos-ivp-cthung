/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PlayAudio.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PlayAudio.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PlayAudio::PlayAudio()
{
  m_playBackDevice = "default";
  m_dir = 0;
  m_playTime = 5;
  m_sampleRate = 48000;
  m_frames = 480;
  m_period_size = 0;
  m_channels = 1;
  m_bits = 16;
  m_loops = 1;
 setPlayParams(); 
}

//---------------------------------------------------------
// Destructor

PlayAudio::~PlayAudio()
{
  snd_pcm_drain(m_handle);
  snd_pcm_close(m_handle);
  free(m_period_buffer);
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PlayAudio::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PlayAudio::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PlayAudio::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  if(m_loops > 0){
    PlayBack();
  }
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PlayAudio::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "FOO") {
      handled = true;
    }
    else if(param == "BAR") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void PlayAudio::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool PlayAudio::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}



void PlayAudio::setPlayParams(){/*{{{*/
  int rc;
  /* Open PCM device for recording. */
  rc = snd_pcm_open(&m_handle, m_playBackDevice.c_str(), SND_PCM_STREAM_PLAYBACK, 0);
  if (rc < 0){
    reportRunWarning("Can't open play back device: " + m_playBackDevice);
  }else
    reportEvent("Open device sucess: " + m_playBackDevice);
  
  /* Allcoate a hardware params object. */ 
  snd_pcm_hw_params_alloca(&m_params);

  /* Fill it in with default values. */ 
  snd_pcm_hw_params_any(m_handle, m_params);

  /* Set the params to device. */
  /*Interleaved mode. */
  snd_pcm_hw_params_set_access(m_handle, m_params, SND_PCM_ACCESS_RW_INTERLEAVED);
  
  /* Signed 16-bit little-endian format. */
  switch(m_bits){
    case 16:
      snd_pcm_hw_params_set_format(m_handle, m_params, SND_PCM_FORMAT_S16_LE);
      break;
    case 24:
      snd_pcm_hw_params_set_format(m_handle, m_params, SND_PCM_FORMAT_S24_LE);
      break;
    case 32:
      snd_pcm_hw_params_set_format(m_handle, m_params, SND_PCM_FORMAT_S32_LE);
      break;
  }
  /* Set channel. */
  snd_pcm_hw_params_set_channels(m_handle, m_params, m_channels);

  /* Set sample rate. */
  snd_pcm_hw_params_set_rate_near(m_handle, m_params, &m_sampleRate, &m_dir);
  
  /* Set period size to frames. */
  snd_pcm_hw_params_set_period_size_near(m_handle, m_params, &m_frames, &m_dir);

  /* Write the params to the driver. */
  rc = snd_pcm_hw_params(m_handle, m_params);
  if(rc < 0)
    reportRunWarning("Unable to set hw params. ");
  else
    reportEvent("Set params sucess. ");
    
  /* Decide the period size and buffer. */
  snd_pcm_hw_params_get_period_size(m_params, &m_frames, &m_dir);
  m_period_size = m_frames*m_bits*m_channels/8; //16bits // units is byte.
  m_period_buffer = (char *) malloc(m_period_size);
}/*}}}*/

void PlayAudio::PlayBack()
{
  int rc;
//  m_loops--;
  rc = read(0, m_period_buffer, m_period_size);
  if(rc == 0){
    reportEvent("End of file on input");
  }else if(rc != m_period_size){
    reportConfigWarning("Short read");
  }

  if(rc == -EPIPE){
    reportConfigWarning("Underrun occurred.");
    snd_pcm_prepare(m_handle);
  }else if(rc < 0){
    string error = snd_strerror(rc);
    reportRunWarning("Error from writei: " + error);
  }else if(rc != (int)m_frames){
    reportConfigWarning("Short read, read wrong frames: " + rc);
  }
}
