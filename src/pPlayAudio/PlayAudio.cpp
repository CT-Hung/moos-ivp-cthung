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
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//---------------------------------------------------------
// Constructor

PlayAudio::PlayAudio()
{
    m_start_play = "false";
    m_total_play = 0;
    m_sh_file = "./play.sh";
    m_play_times = 0;
    m_play_duration = 0;
    m_play_inf = true;
    m_times = 1;
    m_flag_duration = false;
}

//---------------------------------------------------------
// Destructor

PlayAudio::~PlayAudio()
{
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
    string sval   = msg.GetString();
    double dval   = msg.GetDouble();
#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "PLAY_AUDIO"){
       m_start_play = sval;
       m_start_time = MOOSTime();
     } 

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
  if (m_flag_duration == true && m_end_time-m_start_time >= m_play_duration){
    play();
  }else if(m_flag_duration == false){
    play();
  }

  m_end_time = MOOSTime();
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
    if(param == "SH_FILE") {
        m_sh_file = value;
        handled = true;
    }
    else if(param == "PLAY_TIMES") {
        m_times = atof(value.c_str());
        m_play_inf = false;
      handled = true;
    }
    else if(param == "DURATION") {
        m_play_duration = atof(value.c_str());
        m_flag_duration = true;
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
    Register("PLAY_AUDIO", 0);
}

//---------------------------------------------------------
// ProcedureL play 
void PlayAudio::play()
{
  if(m_start_play == "true"){
      if(m_play_inf == false){
        m_total_play++;
        system(m_sh_file.c_str());
        m_play_times++;
        if(m_play_times == m_times){
            m_play_times = 0;
            m_start_play = "false";
            Notify("FINISH_PLAYING", "true")
        }
      }else{
        m_total_play++;
        system(m_sh_file.c_str());
      }
      m_start_time = MOOSTime();
  }
}
//------------------------------------------------------------
// Procedure: buildReport()

bool PlayAudio::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File : "      << m_sh_file         <<       "\n";
  m_msgs << "Play times : "<< m_total_play      <<       "\n";
  m_msgs << "Duration : "  << m_play_duration   <<       "\n";
  if(m_play_inf == false)
    m_msgs << "Play Infinit = false \n";
  else 
    m_msgs << "Play Infinit = true \n";
  
  m_msgs << "============================================ \n";


  return(true);
}


