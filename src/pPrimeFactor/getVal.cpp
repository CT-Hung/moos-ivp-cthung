#include "PrimeFactor.h"
#include <sstream>
#include <cstdlib>

using namespace std;

//Get orig 

string PrimeFactor::getOR() const{
	stringstream ss;
	ss.clear();
	ss << "orig=" << m_input << "receive=" << m_receive;
return(ss.str());
}

//Get time

void PrimeFactor::getTime(){
	m_endTime = MOOSTime();
	m_solve_time = m_endTime - m_startTime;
}

//Get out
void PrimeFactor::getOut(){
	if(m_flag == 1 && m_orig_prime != m_test)
	{
		m_cal++;
		m_test = m_orig_prime;
		stringstream temstr;
		temstr << m_orig_prime << ",cal=" << m_cal;
		temstr << ",solveTime=" << m_solve_time << ",username=CTHung";
		m_outLast = temstr.str();
//		cout << m_outLast << endl;
	}else if (m_flag == 0 && m_orig_prime != m_test){
		m_cal++;
		m_test = m_orig_prime;
		stringstream temstr;
		temstr << m_orig_prime << ",cal=" << m_cal;
		temstr << ",solveTime=" << m_solve_time << ",username=CTHung";
		m_out = temstr.str();
//		cout << m_out << endl;
	}
}

