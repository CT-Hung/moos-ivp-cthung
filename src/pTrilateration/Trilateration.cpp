/************************************************************/
/*    NAME: cthung                                              */
/*    ORGN: MIT                                             */
/*    FILE: Trilateration.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "Trilateration.h"

using namespace std;

//---------------------------------------------------------
// Constructor

Trilateration::Trilateration()
{
	m_v_current_time = 0.0;
	m_v_past_time = 0.0;
	m_v_heading = 0.0;
	m_v_speed = 0.0;
	m_v_posx = 0.0;
	m_v_posy = -10.0;
	m_b1_posx = 300;
	m_b1_posy = 100;
	m_b1_r = 0.0;
	m_b2_posx = 100;
	m_b2_posy = -200;
	m_b2_r = 0.0;
	m_b3_posx = -300;
	m_b3_posy = -100;
	m_b3_r = 0.0;

	m_v_name = "zeta_TRILAT";

	m_b_buffer.clear();

}

//---------------------------------------------------------
// Destructor

Trilateration::~Trilateration()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Trilateration::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
		string sval   = msg.GetString();
		double dval   = msg.GetDouble();
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
		 		m_v_heading = dval;

		 else if(key == "NAV_SPEED")
		 		m_v_speed = dval;

			else if(key == "BRS_RANGE_REPORT")
				BeaconInfo(sval);

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Trilateration::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Trilateration::Iterate()
{
  AppCastingMOOSApp::Iterate();

	DeadReckoning();

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Trilateration::OnStartUp()
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
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "FOO") {
      handled = true;
    }
    else if(param == "BAR") {
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

void Trilateration::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();

	Register("NAV_HEADING", 0);
	Register("NAV_SPEED", 0);
	Register("BRS_RANGE_REPORT", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool Trilateration::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";
	m_msgs << "Vehicle Info:                                \n";
	m_msgs << "HEADING:                " << m_v_heading << endl;
	m_msgs << "SPEED:                  " << m_v_speed << endl;
	m_msgs << "POSX:                   " << m_v_posx << endl;
	m_msgs << "POSY:                   " << m_v_posy << endl;
	m_msgs << "TIME:                   " << m_v_current_time << endl;
	m_msgs << "P1_RANGE:               " << m_b1_r << endl;
	m_msgs << "P2_RANGE:               " << m_b2_r << endl;
	m_msgs << "P3_RANGE:               " << m_b3_r << endl;
  return(true);
}


