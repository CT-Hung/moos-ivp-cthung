/************************************************************//*{{{*/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: CheckSound.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "CheckSound.h"
#include <math.h>

using namespace std;

//---------------------------------------------------------
// Constructor

CheckSound::CheckSound()
{
  m_SampleRate = 48000;
  m_SEL = 0;
  m_sen = 0;
  m_stop = 1;
  m_start = 0;
  m_SEL_time = 1;
  m_real_time_SEL = 0;
  m_real_time_SPLrms = 0;
  m_input_buffer.clear();
  m_pressure_buffer.clear();
  m_SEL = 0;
  m_max_SEL = 0;
  m_max_SPLrms = 0;
  m_bits = 16;
}
//---------------------------------------------------------
// Destructor

CheckSound::~CheckSound()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool CheckSound::OnNewMail(MOOSMSG_LIST &NewMail)
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

     if(key == "Sound_Data"){
       m_input_buffer.push_back(msg.GetString());
     }else if(key == "START_CHECK"){
        if(msg.GetString() == "true"){
          m_stop = 0;
          m_start = 1;
//          m_input_buffer.clear();
//          m_pressure_buffer.clear();
        }
     }

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool CheckSound::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool CheckSound::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  if(m_start == 1){
    ChangeToPressure();
    CalSEL();
  }
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------/*{{{*//*{{{*/
// Procedure: OnStartUp()
//            happens before connection is open

bool CheckSound::OnStartUp()
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
    if(param == "SEN"){
      m_sen = atof(value.c_str());
      handled = true;
    }
    else if(param == "MAX_SEL") {
      m_SEL = atof(value.c_str());
      handled = true;
    }else if(param == "SAMPLE_RATE") {
      m_SampleRate = atoi(value.c_str());
      handled = true;
    }else if(param == "SEL_DURATION") {
      m_SEL_time = atof(value.c_str());
      handled = true;
    }else if(param == "BITS"){
      m_bits = atoi(value.c_str());
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

void CheckSound::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("Sound_Data", 0);
  Register("START_CHECK", 0);
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool CheckSound::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "Threshold_SEL    :       " << m_SEL              << "\n";
  m_msgs << "Real_Time_SEL    :       " << m_real_time_SEL    << "\n";
  m_msgs << "Real_Time_SPLrms :       " << m_real_time_SPLrms << "\n";
  m_msgs << "Max_SEL          :       " << m_max_SEL          << "\n";
  m_msgs << "Max_SPLrms       :       " << m_max_SPLrms       << "\n";
  m_msgs << "Sensitivity      :       " << m_sen              << "\n";
  m_msgs << "SEL_Duration     :       " << m_SEL_time         << "\n";
  m_msgs << "============================================ \n";


  return(true);
}/*}}}*/

void CheckSound::ChangeToPressure()
{
  if(!m_input_buffer.empty()){
    std::string input = m_input_buffer.front();
    m_input_buffer.pop_front();
    vector<string> value = parseString(input, ',');
//    cout << value.size() << endl;
    for(int i=0; i<value.size(); i++){
      stringstream ss;
      long double num = atoi(value[i].c_str());
      double volt;
      switch(m_bits){
        case 16:
          if(num <= 0){
            volt = num/32768;
          }
          else{
            volt = num/32767;
          }
          break;
        case 24:
          if(num <= 0){
            volt = num/8388608;
          }
          else{
            volt = num/8388607;
          }
          break;
        case 32:
          if(num <= 0){
            volt = num/2147483648;
          }
          else{
            volt = num/2147483647;
          }
          break;
      }

      volt = volt/(pow(10, (m_sen/20)));
      
      if(m_pressure_buffer.size() >= (m_SampleRate*m_SEL_time))
        m_pressure_buffer.pop_front();

      m_pressure_buffer.push_back(volt);
    }
  }
}

void CheckSound::CalSEL()
{
  double total = 0;
  if(!m_pressure_buffer.empty()){
    for(int i=0; i<m_pressure_buffer.size(); i++)
     total = total+pow(m_pressure_buffer.at(i), 2);
    
    total = total/m_SampleRate;
    m_real_time_SEL = 10*log10(total)+0.5; //四捨五入
    m_real_time_SPLrms = 10*log10(total/m_SEL_time)+0.5; //四捨五入

  if(m_real_time_SEL > m_max_SEL)
    m_max_SEL = m_real_time_SEL;
  else if(m_real_time_SPLrms > m_max_SPLrms)
    m_max_SPLrms = m_real_time_SPLrms;

  Notify("SEL", m_real_time_SEL);
  Notify("SPLRMS", m_real_time_SPLrms);

    if(m_real_time_SEL >= m_SEL && m_stop == 0){
      reportEvent("Over!!");
      m_stop = 1;
      Notify("START_RECORD", "true");
    }
  }
}

