/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PointAssign_HEADER
#define PointAssign_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <string>
#include <deque>
class PointAssign : public AppCastingMOOSApp
{
 public:
   PointAssign();
   ~PointAssign();
 protected:
 	 std::string m_dec;
 	 std::deque<std::string> m_buffer;
   std::string m_name1;
   std::string m_name2;
	 std::string m_point;
	 std::string m_stop;
	 std::deque<std::string> m_point1;
	 std::deque<std::string> m_point2;
	 int m_c;
	 int m_n1;
	 int m_n2;
	 int m_start;
	 int m_rece;
	 void AssignByRegion();
	 void AssignById();
 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
	 bool	buildReport();
};

#endif 
