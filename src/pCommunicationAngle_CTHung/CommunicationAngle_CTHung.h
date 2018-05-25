/************************************************************/
/*    NAME: CT_Hung                                              */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle_CTHung.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef CommunicationAngle_CTHung_HEADER
#define CommunicationAngle_CTHung_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <math.h>
#include <iostream>
class CommunicationAngle_CTHung : public CMOOSApp
{
 public:
   CommunicationAngle_CTHung();
   ~CommunicationAngle_CTHung();
 protected:
	 void getPosition();
	 void soundTrack();
	 void tranLoss();
	 std::string m_vName;
	 std::string m_cName;
	 double m_vx;
	 double m_vy;
	 double m_vd;
	 double m_vh;
	 double m_vs;
	 double m_cx;
	 double m_cy;
	 double m_cd;
	 double m_ch;
	 double m_cs;
	 double m_g;
	 double m_c0;
	 double m_depth;
	 double m_t;
	 double m_degreR;
	 double m_r;
	 double m_eleAngle;
	 double m_R;
	 double m_TL;
	 double m_chd;
 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();
	 void RegisterColl();
 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
};

#endif 
