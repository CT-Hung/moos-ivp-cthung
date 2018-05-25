#include "PrimeFactorTester.h"
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

//Get prime

void PrimeFactorTester::getPrime(){
	int primeStart, primeEnd;
	int origStart, origEnd;
	string temPrime;
	string aa = m_input;
	primeStart = aa.find(",prime=")+7;
	primeEnd = aa.find(",cal")-1;
	origStart = aa.find("orig=")+5;
	origEnd = aa.find(",receive=")-1;
	temPrime = aa.substr(primeStart, primeEnd).c_str();
	m_orig = atol(aa.substr(origStart, origEnd).c_str());
	m_prime = parseString(temPrime,':');
}
