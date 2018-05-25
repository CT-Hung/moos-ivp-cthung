/************************************************************/
/*    NAME: Levi Cai                                              */
/*    ORGN: MIT                                             */
/*    FILE: HazardPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "HazardPath.h"
#include "PathUtils.h"
#include <iostream>

//#define VERBOSE

using namespace std;

//---------------------------------------------------------
// Constructor

HazardPath::HazardPath()
{
  m_new_scan_request = false;
  m_new_p2p_request = false;
  m_swath_width = 10.0;
  m_turn_radius = 5.0; // enlargens the scan area by this amount to compensate for turn
  m_x = 0.0;
  m_y = 0.0;
  m_num_scan_requests = 0;
  m_template_width = 50.0;
  m_allow_new_scan_requests = true;

  m_max_time = 0.0;
  m_penalty_missed_hazard = 0.0;
  m_penalty_false_alarm = 0.0;
  m_penalty_max_time_over = 0.0;
  m_penalty_max_time_rate = 0.0;

  m_mission_start_time = 0.0;
  m_mission_started = false;
  m_p2p_speed = 1.5;
}

//---------------------------------------------------------
// Destructor

HazardPath::~HazardPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool HazardPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    string key = msg.GetKey();

    if (key == "UHZ_MISSION_PARAMS"){
      m_new_scan_request = true;
      m_scan_request_str = msg.GetString();
      m_num_scan_requests++;
    } else if (key == "SWATH_WIDTH") {
      //string sval = msg.GetString();
      //istringstream(sval) >> m_swath_width;
      //cout << "just got a new swath width" << endl;
    } else if (key == "HAZARDSET_POINT") {
      m_new_p2p_request = true;
      m_p2p_request_str = msg.GetString();
    } else if (key == "NAV_X") {
      m_x = msg.GetDouble();
    } else if (key == "NAV_Y") {
      m_y = msg.GetDouble();
    } else if (key == "MISSION_STARTED") {
      string sval = msg.GetString();
      //istringstream(sval) >> std::boolalpha >> m_mission_started;
      m_mission_start_time = MOOSTime();
      m_mission_started = true;
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

bool HazardPath::OnConnectToServer()
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

bool HazardPath::Iterate()
{
  if (m_new_scan_request && !m_mission_started){
    m_new_scan_request = false;
    HandleScanRegionRequest();
    //m_mission_started = true;
  }


  if (m_new_p2p_request){
    m_allow_new_scan_requests = false;
    m_new_p2p_request = false;
    HandlePoint2PointRequest();
  }

  // Assume start is (0, 0) for the moment
  if (m_mission_started){
    double dist_from_origin = sqrt(m_x*m_x+m_y*m_y);
    double time_running = MOOSTime()-m_mission_start_time;

    double time_remaining = m_max_time - time_running;
    double max_speed = 2.0;
    double buffer = 800.0;
    if (time_remaining <= dist_from_origin/max_speed + buffer){
      m_Comms.Notify("RETURN", "true");
    }
  }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool HazardPath::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "ASSIGNED_REGION_EW") {
	m_assigned_region_ew = value;
      } else if(param == "ASSIGNED_REGION_NS") {
	m_assigned_region_ns = value;
      } else if(param == "TURN_RADIUS") {
	istringstream(value) >> m_turn_radius;
      } else if(param == "TEMPLATE_WIDTH") {
	istringstream(value) >> m_template_width;
      } else if(param == "SWATH_WIDTH") {
	istringstream(value) >> m_swath_width;
      } else if(param == "P2P_SPEED") {
	istringstream(value) >> m_p2p_speed;
      }
    }
  }

  RegisterVariables();	
  return(true);
}

void HazardPath::ParseScanRegionRequest(){
  // TODO: Make this function parse request into a request object
  // Also have it take in params, rather than use state variables
  string penalty_missed_hazard_str = tokStringParse(m_scan_request_str,"penalty_missed_hazard",',','=');
  string penalty_false_alarm_str = tokStringParse(m_scan_request_str,"penalty_false_alarm",',','=');
  string max_time_str = tokStringParse(m_scan_request_str,"max_time",',','=');
  string penalty_max_time_over_str = tokStringParse(m_scan_request_str,"penalty_max_time_over",',','=');
  string penalty_max_time_rate_str = tokStringParse(m_scan_request_str,"penalty_max_time_rate",',','=');
  int bracketi = m_scan_request_str.find('{');
  int bracketf = m_scan_request_str.find('}');
  string pts = m_scan_request_str.substr(bracketi+1,bracketf-bracketi-1);
  
  istringstream(penalty_missed_hazard_str) >> m_penalty_missed_hazard;
  istringstream(penalty_false_alarm_str) >> m_penalty_false_alarm;
  istringstream(max_time_str) >> m_max_time;
  istringstream(penalty_max_time_over_str) >> m_penalty_max_time_over;
  istringstream(penalty_max_time_rate_str) >> m_penalty_max_time_rate;
  
  m_full_search_region = string2SegList(pts);
  
#ifdef VERBOSE
  cout << "New scan request: " << m_scan_request_str << endl;
  cout << "Search region:" << m_full_search_region.get_spec() << endl;
#endif
}

bool HazardPath::HandleScanRegionRequest(){  
  // Generates a very naive scan pattern
  // Parse full region stats

  ParseScanRegionRequest();
  double max_x = m_full_search_region.get_max_x() + m_turn_radius;
  double min_x = m_full_search_region.get_min_x() - m_turn_radius;
  double max_y = m_full_search_region.get_max_y() + m_turn_radius;
  double min_y = m_full_search_region.get_min_y() - m_turn_radius;
  double center_x = m_full_search_region.get_center_x();
  double center_y = m_full_search_region.get_center_y();
  double start_x;
  double start_y;

  double full_width = max_x - min_x;
  double full_height = max_y - min_y;

  // width > height, split it NS to minimize number of turns
  bool ns = full_width > full_height;

  if (ns && m_assigned_region_ns == "NORTH"){
    min_y = center_y + m_swath_width/2.0;
    center_y = (center_y + max_y)/2.0;
    start_x = min_x;
    start_y = max_y;
  } else if (ns && m_assigned_region_ns == "SOUTH") {
    max_y = center_y - m_swath_width/2.0;
    center_y = (center_y + min_y)/2.0;
    start_x = min_x;
    start_y = min_y;
  } else if (!ns && m_assigned_region_ew == "EAST") {
    min_x = center_x + m_swath_width;
    center_x = (center_x + max_x)/2.0;
    start_x = max_x;
    start_y = max_y;
  } else if (!ns && m_assigned_region_ew == "WEST") {
    max_x = center_x - m_swath_width/2.0;
    center_x = (center_x + min_x)/2.0;
    start_x = min_x;
    start_y = max_y;
  }

  double height = max_y - min_y;
  double width = max_x - min_x;

  // Build message packet
  ostringstream lawnmower;
  lawnmower << "points = format=lawnmower";
  lawnmower << ",x=" << center_x;
  lawnmower << ",y=" << center_y;
  lawnmower << ",height=" << height;
  lawnmower << ",width=" << width;
  lawnmower << ",lane_width=" << max(m_swath_width,1.0);
  if (ns){
    lawnmower << ",rows=" << "ew";
  } else {
    lawnmower << ",rows=" << "ns";
  }
  lawnmower << ",startx=" << start_x;
  lawnmower << ",starty=" << start_y;

  m_Comms.Notify("UPDATES_SURVEY_PATH", lawnmower.str());
}

void HazardPath::ParsePoint2PointRequest(){
  int bracketi = m_p2p_request_str.find('{');
  int bracketf = m_p2p_request_str.find('}');
  string pts = m_p2p_request_str.substr(bracketi+1,bracketf-bracketi-1);
  m_all_hazardpts = string2SegList(pts);
}

bool HazardPath::HandlePoint2PointRequest(){
  // TODO: We can gather all points that are close to each other
  // and treat it as a "connected components" problem, just do lawnmower
  // around large connected components

  ParsePoint2PointRequest();
  
  double centroid_x = m_all_hazardpts.get_centroid_x();
  double centroid_y = m_all_hazardpts.get_centroid_y();

  int num_pts = m_all_hazardpts.size();

  double vx, vy;

  // Get assigned points
  m_assigned_hazardpts.clear();
  for (int i=0; i < num_pts; ++i){
    if (m_all_hazardpts.get_vx(i) > centroid_x && m_assigned_region_ew == "EAST"){
      vx = m_all_hazardpts.get_vx(i);
      vy = m_all_hazardpts.get_vy(i);


      m_assigned_hazardpts.add_vertex(vx, vy);
    } else if (m_all_hazardpts.get_vx(i) <= centroid_x && m_assigned_region_ew == "WEST"){
      vx = m_all_hazardpts.get_vx(i);
      vy = m_all_hazardpts.get_vy(i);

      m_assigned_hazardpts.add_vertex(vx, vy);
    }
  }

  // Plan the paths
  m_assigned_hazardpts = greedyPath(m_assigned_hazardpts, m_x, m_y);

  // Add scan paths around each point
  double prev_vx = 0.0, prev_vy = 0.0;
  m_templated_hazardpts.clear();
  for (int i=0; i < m_assigned_hazardpts.size(); ++i){
    double gx = m_assigned_hazardpts.get_vx(i);
    double gy = m_assigned_hazardpts.get_vy(i);

    if (prev_vx == gx && prev_vy == gy){
      //cout << "DUPLICATE FOUND" << endl;
      continue;
    }

    prev_vx = gx;
    prev_vy = gy;
    
    double theight = m_swath_width / 4.0;

    // Template
    m_templated_hazardpts.add_vertex(gx-m_template_width, gy+theight);
    m_templated_hazardpts.add_vertex(gx+m_template_width, gy+theight);
    m_templated_hazardpts.add_vertex(gx+m_template_width, gy-theight);
    m_templated_hazardpts.add_vertex(gx-m_template_width, gy-theight);
  }

  // Notify the new waypoints and deploy
  m_Comms.Notify("UPDATES_SURVEY_PATH", "points="+m_templated_hazardpts.get_spec());

  ostringstream oss;
  oss << m_p2p_speed;
  m_Comms.Notify("UPDATES_SURVEY_PATH", "speed="+oss.str());
  m_Comms.Notify("RESTART","true");
  m_Comms.Notify("DEPLOY","true");
  cout << m_assigned_hazardpts.get_spec() << endl;
  return true;
  
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void HazardPath::RegisterVariables()
{
  m_Comms.Register("UHZ_MISSION_PARAMS", 0);
  m_Comms.Register("SWATH_WIDTH", 0);
  m_Comms.Register("NAV_X", 0);
  m_Comms.Register("NAV_Y", 0);
  m_Comms.Register("HAZARDSET_POINT", 0);
  m_Comms.Register("MISSION_STARTED", 0);
  //  m_Comms.Register();
  // Register("FOOBAR", 0);
}

