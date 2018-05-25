/************************************************************/
/*    NAME: CT-Hung                                              */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Odometry.h"
#include <math.h>
using namespace std;

//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{
	m_iterations = 0;
	m_timewarp   = 1;
	m_current_x = 0;
	m_current_y = 0;
	m_previous_x = 0;
	m_previous_y = 0;
	m_total_distance = 0;
}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
{
	AppCastingMOOSApp::OnNewMail(NewMail);
	MOOSMSG_LIST::iterator p;	
	m_previous_y = m_current_y;
	m_previous_x = m_current_x;
	for(p=NewMail.begin(); p!=NewMail.end(); p++) {
		CMOOSMsg &msg = *p;
		string key = msg.GetKey();
		if(key == "NAV_X"){
			m_current_x = msg.GetDouble();
		}
		if(key == "NAV_Y"){ 
			m_current_y = msg.GetDouble();
		}
	}

	return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
{

	RegisterVariables();
	return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Odometry::Iterate()
{
	AppCastingMOOSApp::Iterate();
	double distance = sqrt((m_current_x - m_previous_x)*(m_current_x - m_previous_x)+(m_current_y - m_previous_y)*(m_current_y - m_previous_y));
	m_total_distance = m_total_distance + distance;
	Notify("ODOMETRY_DIST", m_total_distance); 
	m_iterations++;
    cout << MOOSTime() << endl;
	AppCastingMOOSApp::PostReport();
	return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
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

void Odometry::RegisterVariables()
{
	AppCastingMOOSApp::RegisterVariables();
	// Register("FOOBAR", 0);
	Register("NAV_X", 0);
	Register("NAV_Y", 0);
}

bool Odometry::buildReport()
{
	m_msgs << "Distance" << m_total_distance << endl;

	return(true);
}
