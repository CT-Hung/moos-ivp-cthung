#include "PrimeFactor.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace std;

void PrimeFactor::findPrime(){
	unsigned long int orig, tem;
	int receive;
	stringstream primes;
	stringstream temstr;
		string aa = m_orig_rec; 
		int pos = aa.find("receive=");
		int num = 1;
		orig = atol(aa.substr(5, pos-1).c_str());
		receive = atoi(aa.substr(pos+8).c_str());
		tem = orig;
		for(unsigned long int k = 2; k <= 10000000; k++){
			if(tem % k == 0){
				for(unsigned long int j = 2; j <= k; j++){
					if(k == j){
						primes << k << ":";
						for(int c = 0; c <= 20; c++){
							if(tem%k != 0){
								break;
							}
							tem = tem/k;
						}
					}else if(k%j == 0){
						break;
					}
				}
			}else if(tem == 1){
				break;
			}
		}
		if(tem == 1){
			temstr << "orig=" << orig << ",receive=" << receive;
			temstr << ",prime=" << primes.str();
			m_orig_prime = temstr.str();
			m_flag = 0;
		}else if(tem != 1){
 			temstr << "orig=" << orig << ",receive=" << receive;
			temstr << ",prime=" << primes.str() << ",last1=" << tem;
			temstr << ",num=" << num;
			m_orig_prime_last1.push_back(temstr.str());
	//		cout << temstr.str() << endl;
		}
	}

void PrimeFactor::findLast1prime(){
	int pos, posn;
	unsigned long int num;
	stringstream prime;
	stringstream temstr;
	string tmpstring;
	unsigned long int last1;
	list<string>::iterator index;
	string fi1, fl1;
	index = m_orig_prime_last1.begin();
	fi1 = m_orig_prime_last1.front();
	pos = fi1.find(",last1=");
	posn = fi1.find(",num=");
	last1 = atol(fi1.substr(pos+7).c_str());
	num = atol(fi1.substr(posn+5).c_str());
	for(unsigned long long int i = 10000000*num+1; i <= 10000000*num+20000000; i=i+2){
		if(last1%i == 0){
			for(unsigned long long int j = 2; j <= i; j++){
				if(i == j){
					prime << i << ":";
					for(int c = 0; c <= 20; c++){
						last1 = last1/i;
						if(last1%i != 0){
							break;
						}
					}
				}else if(i%j == 0){
					break;
				}
			}
		}else if(last1 == 1){
			break;
		}
	}	int ss = fi1.size();
	fi1.erase(pos, ss);
	temstr << fi1 << prime.str();
	m_orig_prime_last1.erase(index);
	if(last1 == 1){
		m_orig_prime = temstr.str();
		m_flag = 1;
	}else if(last1 != 1){
		num = num + 2;
		temstr << ",last1=" << last1 << ",num=" << num;
		m_orig_prime_last1.push_front(temstr.str());
	//	cout << temstr.str() << endl;
	}

}

