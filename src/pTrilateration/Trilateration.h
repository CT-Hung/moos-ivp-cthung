/************************************************************/
/*    NAME: cthung                                              */
/*    ORGN: MIT                                             */
/*    FILE: Trilateration.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef Trilateration_HEADER
#define Trilateration_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
class Trilateration : public AppCastingMOOSApp
{
 public:
   Trilateration();
   ~Trilateration();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
 
 protected: // New functions
 	 void DeadReckoning();
	 void BeaconInfo(std::string);
 protected: // New varibales 
 	 double m_v_current_time;
	 double m_v_past_time;
	 double m_v_heading;
	 double m_v_speed;
	 double m_v_posx;
	 double m_v_posy;
	 double m_b1_posx;
	 double m_b1_posy;
	 double m_b1_r;
	 double m_b2_posx;
	 double m_b2_posy;
	 double m_b2_r;
	 double m_b3_posx;
	 double m_b3_posy;
	 double m_b3_r;
	 std::string m_v_name;

	 std::deque<std::string> m_b_buffer;
 private: // Configuration variables

 private: // State variables
};

#endif 
