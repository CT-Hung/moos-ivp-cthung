#include "PrimeFactorTester.h"
#include <sstream>
using namespace std;

void PrimeFactorTester::primeTest(){
	stringstream ss;
	unsigned long int test = m_orig;
	for(int i = 0; i < m_prime.size(); i++){
		unsigned long int prime = atol(m_prime[i].c_str());
		for(int j = 0; j <= 100; j++){
			test = test/prime;
			if(test%prime != 0){
				break;
			}
		}
		if(test == 1){
			break;
		}
	}
	if(test == 1){
		ss << m_input << "valid=true";
		m_out = ss.str();
		m_prime.clear();
	}else{
		ss << m_input << "valid=false";
		m_out = ss.str();
		m_prime.clear();
	}
}
