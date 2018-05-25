/************************************************************/
/*    NAME: Levi Cai                                              */
/*    ORGN: MIT                                             */
/*    FILE: HazardPath.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef HazardPath_HEADER
#define HazardPath_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "GeomUtils.h"
#include "XYSegList.h"
#include "XYFormatUtilsSegl.h"
//#include "XYPolygon.h"
//#include "XYFormatUtilsPoly.h"

class HazardPath : public CMOOSApp
{
 public:
   HazardPath();
   ~HazardPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();
   void ParseScanRegionRequest();
   void ParsePoint2PointRequest();
   bool HandleScanRegionRequest();
   bool HandlePoint2PointRequest();
   
 private: // Configuration variables

 private: // State variables
   double m_penalty_missed_hazard;
   double m_penalty_false_alarm;
   double m_max_time;
   double m_penalty_max_time_over;
   double m_penalty_max_time_rate;
   double m_swath_width;
   double m_turn_radius;
   double m_x;
   double m_y;
   double m_template_width;
   double m_mission_start_time;
   double m_p2p_speed;
   int m_num_scan_requests;
   bool m_allow_new_scan_requests;
   bool m_mission_started;
   XYSegList m_full_search_region;
   XYSegList m_assigned_search_region;
   XYSegList m_all_hazardpts;
   XYSegList m_assigned_hazardpts;
   XYSegList m_templated_hazardpts;
   bool m_new_scan_request;
   bool m_new_p2p_request;
   std::string m_scan_request_str;
   std::string m_p2p_request_str;
   std::string m_assigned_region_ew;
   std::string m_assigned_region_ns;

};

#endif 
