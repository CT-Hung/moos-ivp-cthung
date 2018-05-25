/************************************************************/
/*    NAME: CT-Hung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER
#include "stdint.h"
#include "MOOS/libMOOS/MOOSLib.h"
#include <vector>
#include <string>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <list>
#include <deque>
class PrimeFactor : public CMOOSApp
{
 public:
   PrimeFactor();
   ~PrimeFactor();

 protected:  //get val	
	 std::string getOR() const;
	 void getTime();
	 void getOut();
	 void findPrime();
	 void findLastprime();
	 void findLast1prime();	
	 void findLast2prime(); 	
	 void findLast3prime();
	 std::string m_orig_rec;
	 std::string m_orig_prime, m_test, m_test1;
	 std::list<std::string> m_orig_prime_last1;
	 std::deque<std::string> m_buffer;
	 std::deque<std::string> m_buffer2;
	 std::string  m_testLast, m_testLast1;
	 std::string m_outLast;
	 std::string m_out;
	 int m_flag;
	 int m_receive;
	 std::string m_orig;
	 time_t m_startTime;
	 double m_endTime;
	 double m_solve_time;
	 int m_cal;
 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();
	 std::string m_input;

 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
};

#endif 
