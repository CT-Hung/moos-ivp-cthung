/************************************************************/
/*    NAME: CT-Hung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PrimeFactor.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <time.h>
using namespace std;

//---------------------------------------------------------
// Constructor
   
PrimeFactor::PrimeFactor()
{
  m_iterations = 0;
  m_timewarp   = 1;
	m_input = 3;
	m_startTime = MOOSTime();
	m_endTime = 0.0;
	m_receive = 0;
	m_cal = 0;
	m_flag = 3;
	m_orig_prime_last1.clear();
//  m_orig_primeLast = "";
}

//---------------------------------------------------------
// Destructor

PrimeFactor::~PrimeFactor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key = msg.GetKey();
		if(key == "NUM_VALUE"){
			m_buffer.push_back(msg.GetString());
	  }
	}	
   return(true);
}


//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactor::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactor::Iterate()
{	
  m_iterations++;
	if(!m_buffer.empty()){
		m_input = m_buffer.front();
		m_buffer.pop_front();
		m_receive++;
		m_orig_rec = getOR();
		findPrime();
	getTime();
	getOut();
	}
	if(!m_orig_prime_last1.empty()){
	  findLast1prime();
		getTime();
		getOut();
	}
	if(m_outLast != m_testLast1){
		m_testLast1 = m_outLast;
		Notify("PRIME_RESULT", m_outLast);
	}
	if(m_out != m_test1){
		m_test1 = m_out;
		Notify("PRIME_RESULT", m_out);
	}
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactor::OnStartUp()
{
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

void PrimeFactor::RegisterVariables()
{
  // Register("FOOBAR", 0);
	Register("NUM_VALUE", 0 );

}
