/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: BearingAngle.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "BearingAngle.h"

using namespace std;

//---------------------------------------------------------
// Constructor

BearingAngle::BearingAngle()
{
}

//---------------------------------------------------------
// Destructor

BearingAngle::~BearingAngle()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool BearingAngle::OnNewMail(MOOSMSG_LIST &NewMail)
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

     if(key == "NAV_HEADING")
         m_b_h = msg.GetDouble();
     else if(key == "NAV_X")
         m_b_x = msg.GetDouble();
     else if(key == "NAV_Y")
         m_b_y = msg.GetDouble();
     else if(key == "M1_TIME")
         m_m1_rt = msg.GetDouble();
     else if(key == "M2_TIME")
         m_m2_rt = msg.GetDouble();
     else if(key == "M3_TIME")
         m_m3_rt = msg.GetDouble();

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool BearingAngle::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool BearingAngle::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
    micUpdate();

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool BearingAngle::OnStartUp()
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
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "center_to_mic") {
        m_b_m = atof(value.c_str());
        handled = true;
    }
    else if(param == "sound_velocity") {
        m_c = atof(value.c_str());
        handled = true;
    }
    else if(param == "fs"){
        m_fs = atof(value.c_str());
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

void BearingAngle::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
    Register("NAV_X", 0);
    Register("NAV_Y", 0);
    Register("NAV_HEADING", 0);
    Register("M1_TIME", 0);
    Register("M2_TIME", 0);
    Register("M3_TIME", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool BearingAngle::buildReport() 
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

void BearingAngle::micUpdate()
{
    double h;
    h = m_b_h/180*PI;
    m_m1_h = h-PI/3;
    m_m2_h = h+PI/3;
    m_m3_h = h+PI;
    m_m1_x = m_b_x+sin(m_m1_h)*m_b_m;
    m_m1_y = m_b_y+cos(m_m1_h)*m_b_m;
    m_m2_x = m_b_x+sin(m_m2_h)*m_b_m;
    m_m2_y = m_b_y+cos(m_m2_h)*m_b_m;
    m_m3_x = m_b_x+sin(m_m3_h)*m_b_m;
    m_m3_y = m_b_y+cos(m_m3_h)*m_b_m;
    m_m1_m2_dl = sqrt(pow((m_m1_x-m_m2_x), 2)+pow((m_m1_y-m_m2_y), 2));
}

void BearingAngle::TDOA()
{

}

void BearingAngle::Bearing2Mic()
{
    double dt;
    double theta;
    dt = m_m1_rt - m_m2_rt;
    theta = 90-acos(m_c/m_m1_m2_dl)*180/PI;
    theta = theta+m_b_h*180/PI;
    if (theta < 0)
        theta = theta+360;
    m_source_guessAngle = round(theta);
}
