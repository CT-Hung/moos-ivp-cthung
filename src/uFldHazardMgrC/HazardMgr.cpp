
#include <iterator>
#include "MBUtils.h"
#include "HazardMgr.h"
#include "XYFormatUtilsHazard.h"
#include "XYFormatUtilsPoly.h"
#include "ACTable.h"

using namespace std;

//---------------------------------------------------------
// Constructor

HazardMgr::HazardMgr()
{
  // Config variables
  m_swath_width_desired = 25;
  m_pd_desired          = 0.9;

  // State Variables 
  m_sensor_config_requested = false;
  m_sensor_config_set   = false;
  m_swath_width_granted = 0;
  m_pd_granted          = 0;

  m_sensor_config_reqs = 0;
  m_sensor_config_acks = 0;
  m_sensor_report_reqs = 0;
  m_detection_reports  = 0;
  m_summary_reports = 0;
	m_final_report = "";
	m_restart_sep = "";
	m_point_path = "";
	m_report = "";
	m_pt = "";
	m_restart_merge = "false";
	m_restart_post = "false";
	m_restart_final = "false";
	m_restart = "false";
	m_start = "false";
	m_check_twice = "true";
	m_post_time = 0.0;
	m_finish_merge = false;
	m_mission_end = false;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool HazardMgr::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    string sval  = msg.GetString(); 

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    
    if(key == "UHZ_CONFIG_ACK") 
      handleMailSensorConfigAck(sval);

    else if(key == "UHZ_OPTIONS_SUMMARY") 
      handleMailSensorOptionsSummary(sval);

    else if(key == "UHZ_DETECTION_REPORT") 
      handleMailDetectionReport(sval);

    else if(key == "HAZARDSET_REQUEST") 
      handleMailReportRequest();

    else if(key == "UHZ_MISSION_PARAMS") 
      handleMailMissionParams(sval);
		
		else if(key == "START_COM"){	
			if(m_restart_merge == "false"){
				m_start = sval;
				m_post_time = MOOSTime();
			}else if(m_restart_merge == "true"){
				m_restart_post = sval;
				m_post_time = MOOSTime();
			}
		}
		else if(key == "COLL_REPORT"){
			mergePoint(sval);
	//		m_post_time = MOOSTime();
			if(m_restart == "false"){
				m_post_time = MOOSTime();
			}
			m_restart = "true";
			m_start = "false";
		}
		else if(key == "RESTART"){
			m_restart_merge = sval;
		}
		else if(key == "MERGE_TIME"){
			m_restart_final = sval;
			m_restart_post = "false";
			m_post_time = MOOSTime();
		}
		else if(key == "FINAL_REPORT"){
			m_restart_final = "false";
			m_restart_post = "false";
			mergeReport(sval);
		}
		
		else if(key == "MISSION_END")
			m_mission_end = true;
    else 
      reportRunWarning("Unhandled Mail: " + key);
  }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool HazardMgr::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool HazardMgr::Iterate()
{
  AppCastingMOOSApp::Iterate();
	double incurrent_time;
	double wait_restart;
	double frequency;
	incurrent_time = MOOSTime();
	wait_restart = incurrent_time-m_wait_time;
	frequency = incurrent_time-m_post_time;
	
	if(m_start == "true" && frequency >= 65 && m_mission_end == false){
			Notify("NODE_MESSAGE_LOCAL", m_point_path);
			m_post_time = incurrent_time;
		}
	if(m_restart == "true" && m_check_twice == "true" && frequency >= 65 && m_mission_end == false){
		m_pd_desired = 0.8;
		Notify("NODE_MESSAGE_LOCAL", m_point_path);
		if(frequency >= 140){
			Notify("NODE_MESSAGE_LOCAL", m_point_path);
			Notify("HAZARDSET_POINT", m_restart_point);
			m_check_twice = "false";
			m_start = "false";
			reportEvent(m_restart_point);
		}
	}
	if(frequency >= 140 && m_mission_end == false){
		if(m_restart_post == "true"){
			postRePost();
			m_post_time = MOOSTime();
		}else if(m_restart_final == "true"){
			postFinalReport();
			m_post_time = MOOSTime();
		}
	}

  if(!m_sensor_config_requested)
    postSensorConfigRequest();

  if(m_sensor_config_set)
    postSensorInfoRequest();

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool HazardMgr::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(true);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if((param == "swath_width") && isNumber(value)) {
      m_swath_width_desired = atof(value.c_str());
      Notify("SWATH_WIDTH", value);
			handled = true;
    }
    else if(((param == "sensor_pd") || (param == "pd")) && isNumber(value)) {
      m_pd_desired = atof(value.c_str());
      handled = true;
    }
    else if(param == "report_name") {
      value = stripQuotes(value);
      m_report_name = value;
      handled = true;
    }
    else if(param == "region") {
      XYPolygon poly = string2Poly(value);
      if(poly.is_convex())
	m_search_region = poly;
      handled = true;
    }else if(param == "cname"){
			m_cname = value;
			handled = true;
		}

    if(!handled)
      reportUnhandledConfigWarning(orig);
  }
  
  m_hazard_set.setSource(m_host_community);
  m_hazard_set.setName(m_report_name);
  m_hazard_set.setRegion(m_search_region);
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void HazardMgr::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("UHZ_DETECTION_REPORT", 0);
  Register("UHZ_CONFIG_ACK", 0);
  Register("UHZ_OPTIONS_SUMMARY", 0);
  Register("UHZ_MISSION_PARAMS", 0);
  Register("HAZARDSET_REQUEST", 0);
	Register("START_COM", 0);
	Register("COLL_REPORT", 0);
	Register("RESTART", 0);
	Register("MERGE_TIME", 0);
	Register("FINAL_REPORT", 0);
}

//------------------------------------------------------------
// Procedure: buildReport()

bool HazardMgr::buildReport() 
{
  m_msgs << "Config Requested:"                                  << endl;
  m_msgs << "    swath_width_desired: " << m_swath_width_desired << endl;
  m_msgs << "             pd_desired: " << m_pd_desired          << endl;
  m_msgs << "   config requests sent: " << m_sensor_config_reqs  << endl;
  m_msgs << "                  acked: " << m_sensor_config_acks  << endl;
  m_msgs << "------------------------ "                          << endl;
  m_msgs << "Config Result:"                                     << endl;
  m_msgs << "       config confirmed: " << boolToString(m_sensor_config_set) << endl;
  m_msgs << "    swath_width_granted: " << m_swath_width_granted << endl;
  m_msgs << "             pd_granted: " << m_pd_granted          << endl << endl;
  m_msgs << "--------------------------------------------" << endl << endl;

  m_msgs << "               sensor requests: " << m_sensor_report_reqs << endl;
  m_msgs << "             detection reports: " << m_detection_reports  << endl << endl; 

  m_msgs << "   Hazardset Reports Requested: " << m_summary_reports << endl;
  m_msgs << "      Hazardset Reports Posted: " << m_summary_reports << endl;
  m_msgs << "                   Report Name: " << m_report_name << endl;
	m_msgs << "--------------------------------------------" << endl<<endl;
	m_msgs << "                  Final Report: " << m_final_report << endl;
	m_msgs << "                       Restart: " << m_restart_merge << endl;
  return(true);
}








