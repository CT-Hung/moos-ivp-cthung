#include "CommunicationAngle_CTHung.h"
#include <math.h>

using namespace std;

void CommunicationAngle_CTHung::getPosition(){
	m_vx = m_vx + sin(m_vh*PI/180)*m_vs*m_t;
	m_vy = m_vy + cos(m_vh*PI/180)*m_vs*m_t;
	m_cx = m_cx + sin(m_ch*PI/180)*m_cs*m_t;
	m_cy = m_cy + cos(m_ch*PI/180)*m_cs*m_t;
}

void CommunicationAngle_CTHung::soundTrack(){
	double ccz = -m_c0/m_g; //circule center z
	double cz0 = m_c0+m_g*m_vd;
	double ccr; //circule center r
	double R; //circule rad
	double degre0; //fire degree
	double zmax;
	double degrezmax, rzmax;
	ccr = (m_r*m_r-pow((m_vd-ccz), 2)+pow((m_cd-ccz), 2))/(2*m_r);
	R = sqrt(ccr*ccr+pow((m_vd-ccz), 2));
	degre0 = acos(cz0/(m_g*R));
	zmax = cz0/(m_g*cos(degre0))-m_c0/m_g;
	m_degreR = asin(sin(degre0)-m_r/R);
	degrezmax = acos((zmax+m_c0/m_g)/R);
	rzmax = R*(sin(degre0)-sin(degrezmax));
	m_R = R;
	if(zmax < m_depth){
		m_eleAngle = -degre0;
/*	}else if(zmax > m_depth){
	  if(rzmax < m_r)
			m_eleAngle = degre0;
		else{
			m_vd--;
			soundTrack();
		}*/
	}else{
		m_eleAngle = 0;
		zmax = m_depth-0.02*PI/180;
		degre0 = -acos(cz0/(m_g*zmax+m_c0));
		R = cz0/(m_g*cos(degre0));
		ccr = m_r-sqrt(pow(m_cd-ccz, 2)-R*R);
		m_chd = ccz+sqrt(R*R-ccr*ccr);
	}
}

void CommunicationAngle_CTHung::tranLoss(){
	double J;
	double r1;
	double degreR1;
	double degre0;
	double p;
	double R = m_c0+m_g*m_vd;
	double czs;
	double cz0;
	cz0 = m_c0+m_g*m_vd;
	czs = m_c0+m_g*m_cd;
	degre0 = m_eleAngle-0.001*PI/180;
	r1 = m_R*(sin(degre0)-sin(m_degreR));
	J = m_r/(sin(m_degreR))*(r1-m_r)/(0.001*PI/180);
	p = (sqrt(czs*cos(degre0)/(cz0*J)))/(4*PI);
	m_TL = -20*log10(p*4*PI);
}
