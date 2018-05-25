/************************************************************/
/*    NAME: CT_Hung                                              */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle_CTHung.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "CommunicationAngle_CTHung.h"
#include <sstream>
#include <cstdlib>

using namespace std;

//---------------------------------------------------------
// Constructor

CommunicationAngle_CTHung::CommunicationAngle_CTHung()
{ 
	m_vName = "";
	m_cName = "";
	m_vx = 0;
	m_vy = 0;
	m_vd = 0;
	m_vh = 0;
	m_vs = 0;
	m_cx = 0;
	m_cy = 0;
	m_cd = 0;
	m_ch = 0;
	m_cs = 0;
	m_chd = 0;
	m_g = 0.016;
	m_c0 = 1480;
	m_depth = 6000;
	m_t = 0.25;
	m_r = 0;
	m_eleAngle = 0;
	m_degreR = 0;
	m_R = 0;
	m_TL = 0;
  m_iterations = 0;
  m_timewarp   = 1;
}

//---------------------------------------------------------
// Destructor

CommunicationAngle_CTHung::~CommunicationAngle_CTHung()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool CommunicationAngle_CTHung::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
		string key = msg.GetKey();
		if(key == "VEHICLE_NAME"){
			m_vName = msg.GetString();
		}
		if(key == "COLLABORATOR_NAME"){
			m_cName = msg.GetString();
			RegisterColl();
		}
		if(key == "NAV_X"){
			m_vx = msg.GetDouble();
		}
		if(key == "NAV_Y"){
			m_vy = msg.GetDouble();
		}
		if(key == "NAV_DEPTH"){
			m_vd = msg.GetDouble();
		}
		if(key == "NAV_HEADING"){
			m_vh = msg.GetDouble();
		}
		if(key == "NAV_SPEED"){
			m_vs = msg.GetDouble();
		}
		if(key == m_cName+"_NAV_X"){
			m_cx = msg.GetDouble();
		}
		if(key == m_cName+"_NAV_Y"){	
			m_cy = msg.GetDouble();
		}
		if(key == m_cName+"_NAV_DEPTH"){
			m_cd = msg.GetDouble();
		}
		if(key == m_cName+"_NAV_HEADING"){
			m_ch = msg.GetDouble();
		}
		if(key == m_cName+"_NAV_SPEED"){
			m_cs = msg.GetDouble();
		}


#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool CommunicationAngle_CTHung::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool CommunicationAngle_CTHung::Iterate()
{
  m_iterations++;
	m_r = sqrt(pow((m_cx-m_vx), 2)+pow((m_cy-m_vy), 2));
	soundTrack();
	stringstream location;
	stringstream acousticPath;
	if(m_eleAngle != 0){	
		location << "x=" << m_vx << ",y=" << m_vy << ",depth=" << m_vd;
		location << ",id=r04525098@ntu.edu.tw";
		Notify("CONNECTIVITY_LOCATION", location.str());
		tranLoss();
		acousticPath << "elev_angle=" << m_eleAngle*180/PI << ",transmission_loss=";
		acousticPath << m_TL << ",id=r04525098@ntu.edu.tw";
	}else{
		location << "x=" << m_vx << ",y=" << m_vy << ",depth=" << m_chd;
		location << ",id=r04525098@ntu.edu.tw";
		Notify("CONNECTIVITY_LOCATION", location.str());
		acousticPath << "NaN";
	}
	Notify("ACOUSTIC_PATH", acousticPath.str());
	getPosition();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool CommunicationAngle_CTHung::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  m_timewarp = GetMOOSTimeWarp();

  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void CommunicationAngle_CTHung::RegisterVariables()
{
  // Register("FOOBAR", 0);
	Register("VEHICLE_NAME", 0);
	Register("COLLABORATOR_NAME", 0);
	Register("NAV_X", 0);
	Register("NAV_Y", 0);
	Register("NAV_DEPTH", 0);
	Register("NAV_HEADING", 0);
	Register("NAV_SPEED", 0);

}

void CommunicationAngle_CTHung::RegisterColl()
{

	Register(m_cName+"_NAV_X", 0);
	Register(m_cName+"_NAV_Y", 0);
	Register(m_cName+"_NAV_DEPTH", 0);
	Register(m_cName+"_NAV_HEADING", 0);
	Register(m_cName+"_NAV_SPEED", 0);
}
