/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <iostream>
#include <string>
#include <XYSegList.h>
#include <deque>
#include <vector>
class GenPath : public AppCastingMOOSApp
{
 public:
   GenPath();
   ~GenPath();
 protected:
 	 std::string m_update;
   std::string m_point;
	 std::deque<std::string> m_buffer;
	 std::deque<std::string> m_buffer1;
	 std::string m_first;
	 std::string m_last;
	 std::string m_nav;
	 std::string m_gen;
	 std::string m_st_gen;
	 std::string m_r;
	 std::string m_deploy;
	 std::string m_re;
	 std::string m_name;
	 std::deque<std::string> m_check;
	 XYSegList m_seglist;
	 int m_c;
	 int m_in;
	 int m_tx;
	 int m_ty;
	 int m_flag;
	 int m_index;
	 int m_visit;
	 int m_rece;
	 double m_x;
	 double m_y;
	 double m_xx;
	 double m_yy;
	 std::string m_cc;
	 void GetGen();
   void GetXY();
	 void CheckPoint();
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
	 bool buildReport();
};

#endif 
