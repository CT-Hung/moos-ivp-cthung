/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PointAssign.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{	
	m_dec = "a";
	m_point = "";
	m_c = -4;
	m_n1 = -2;
	m_n2 = -2;
	m_start = 0;
	m_rece = 0;
  m_iterations = 0;
  m_timewarp   = 1;
}

//---------------------------------------------------------
// Destructor

PointAssign::~PointAssign()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign::OnNewMail(MOOSMSG_LIST &NewMail)
{
	AppCastingMOOSApp::OnNewMail(NewMail);
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
		string key = msg.GetKey();
		if(key == "VISIT_POINT"){
			string so = msg.GetSource();
			m_buffer.push_back(msg.GetString());
			m_rece++;
			cout <<" FOOBAr " << so << endl;
		}else if(key == "ASS_START"){
			m_start++;
		}
  }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PointAssign::Iterate()
{
	AppCastingMOOSApp::Iterate();
//	if(m_start == 2){
	if(m_dec == "true"){
		AssignByRegion();
	}else{
		AssignById();
	}
	while(!m_point1.empty()){
		Notify("VISIT_POINT_"+toupper(m_name1), m_point1.front());
		m_point1.pop_front();
		m_n1++;
		m_c++;
	}
	while(!m_point2.empty()){
		Notify("VISIT_POINT_"+m_name2, m_point2.front());
		m_point2.pop_front();
		m_n2++;
		m_c++;
	}
	m_point1.clear();
	m_point2.clear();
 	Notify("MAIL_ASS", m_c);
//	}
	m_iterations++;
	AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PointAssign::OnStartUp()
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
      if(param == "ASSIGN_BY_REGION") {
				m_dec = value;
      }
			else if(param == "VNAME1"){
					m_name1 = value;
			}
			else if(param == "VNAME2"){
					m_name2 = value;
			}
    }
  }
  
  m_timewarp = GetMOOSTimeWarp();

  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PointAssign::RegisterVariables()
{
	AppCastingMOOSApp::RegisterVariables();
	Register("VISIT_POINT", 0);
  // Register("FOOBAR", 0);
}

//--------------------------------------------------------
// Procedure: buildReport

bool PointAssign::buildReport(){
	m_msgs << "Total_Points_Rec: " << m_rece  << endl;
	m_msgs << "Total_Points_Ass: " << m_c  << endl;
	m_msgs << m_name1+"_Points:  " << m_n1 << endl;
	m_msgs << m_name2+"_Points:  " << m_n2 << endl;
	m_msgs << "Assign_By_Region: " << m_dec << endl;
	m_msgs << "VNAME1:           " << m_name1 << endl;
	m_msgs << "VNAME2:           " << m_name2 << endl;
	return(true);
}
