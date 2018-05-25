/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "GenPath.h"

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{	
	m_update = "points=";
	m_point = "";
	m_c = 0;
	m_seglist.clear();
	m_check.clear();
	m_tx = 1000;
	m_ty = 1000;
	m_last = "false";
	m_first = "false";
	m_nav = "false";
	m_gen = "false";
	m_st_gen = "false";
	m_deploy = "false";
	m_flag = 0;
	m_in = -2;
	m_x = 0;
	m_y = 0;
  m_iterations = 0;
  m_timewarp   = 1;
	m_index = 0;
	m_visit = 0;
	m_rece = 0;
	m_r = 3;
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
	AppCastingMOOSApp::OnNewMail(NewMail);
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
		string key = msg.GetKey();
		if(key == "VISIT_POINT"){
			m_buffer.push_back(msg.GetString());
		}else if(key == "NAV_X"){
			m_x = msg.GetDouble();
			m_nav = "true";
		}else if(key == "NAV_Y"){
			m_y = msg.GetDouble();
		}else if(key == "GENPATH_REGENERATE"){
			m_gen = msg.GetString();
		}else if(key == "DEPLOY"){
			m_deploy = msg.GetString();
		}else if(key == "WPT_INDEX"){
			m_index = msg.GetDouble();
		}else if(key == "RETURN"){
			m_re = msg.GetString();
		}else if(key == "START_GEN"){
			m_st_gen = msg.GetString();
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

bool GenPath::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
   Notify("ASS_START", "false");
	 RegisterVariables();
	 
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool GenPath::Iterate()
{
	AppCastingMOOSApp::Iterate();
	if(!m_buffer.empty()){
		GetXY();
		if(m_point == "firstpoint"){
			m_xx = m_x;
			m_yy = m_y;
			m_first = "true";
			m_flag = 1;
		}else if(m_point == "lastpoint"){
			m_last = "true";
			m_flag = 2;
			m_buffer.clear();
			m_seglist.clear();
		}else if(m_flag == 1){
			string pt = m_update + m_seglist.get_spec();
			Notify("UPDATE_POINT", pt);
			Notify("UPDATE_POINT", "radius = "+m_r);
		}
	}else if (m_deploy == "true" && m_re == "false"){
		CheckPoint();
	}else if(m_gen == "true"){
		GetGen();
		if(m_c != 0){
			string pt = m_update + m_seglist.get_spec();
			Notify("UPDATE_POINT", pt);
			m_gen = "false";
			m_st_gen = "false";
			Notify("START_GEN", m_st_gen);
			Notify("GENPATH_REGENERATE", m_gen);
			Notify("START_GEN", m_st_gen);
			Notify("RETURN", "false");
			Notify("STATION_KEEP", "false");
			m_seglist.clear();
		}else if(!m_check.empty()){
			m_gen = "false";
			Notify("GEN_RE", m_gen);
			Notify("RETURN", "true");
		}
	}
  m_iterations++;
	AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
{
	AppCastingMOOSApp::OnStartUp();
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "VISIT_RADIUS") {
        m_r = value;
      }
      else if(param == "NAME") {
        m_name = value;
      }
    }
  }
  
  m_timewarp = GetMOOSTimeWarp();

  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void GenPath::RegisterVariables()
{	
	AppCastingMOOSApp::RegisterVariables();
	Register("VISIT_POINT", 0);
	Register("NAV_X", 0);
	Register("NAV_Y", 0);
	Register("VIEW_POINT", 0);
	Register("DEPLOY", 0);
	Register("GENPATH_REGENERATE", 0);
	Register("WPT_INDEX", 0);
	Register("RETURN", 0);
	Register("START_GEN", 0);
}

//--------------------------------------------------------
// Prcedure: buildReport

bool GenPath::buildReport(){
	m_msgs << "Visit_Radius:            " << m_r << endl;
	m_msgs << "Total_Points_Received:   " << m_rece << endl;
	m_msgs << "Invalid_Points_Received: " << m_in << endl;
	m_msgs << "First_Point_Received:    " << m_first << endl;
	m_msgs << "Last_Point_Received:     " << m_last << endl;
	m_msgs << "NAV_X/Y_Received:        " << m_nav << endl;
	m_msgs << "NAME:                    " << m_name << endl;
	m_msgs << endl;
	m_msgs << "Tour_Status              " << endl;
	m_msgs << "-------------------------" << endl;
	m_msgs << "Visit_Points:            " << m_visit << endl;
	m_msgs << "Unvisit_Point:           " << m_c << endl;
	return(true);
}
