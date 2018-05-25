#include "Trilateration.h"
#include "MBUtils.h"
#include "sstream"
#include "string"
#include <stdlib.h>
#include "ACTable.h"
using namespace std;

void Trilateration::DeadReckoning()
{
	double delt_time;
	stringstream msg;
	m_v_current_time = MOOSTime();
	delt_time = m_v_current_time - m_v_past_time;
	m_v_posx = m_v_posx+delt_time*m_v_speed*sin(m_v_heading*PI/180);
	m_v_posy = m_v_posy+delt_time*m_v_speed*cos(m_v_heading*PI/180);
	msg << "NAME=" << m_v_name << ",TYPE=KAYAK";
	msg << ",X=" << m_v_posx << ",Y=" << m_v_posy;
	msg << ",HDG=" << m_v_heading << ",SPD=" << m_v_speed;
	msg << ",TIME=" << fixed << m_v_current_time;
	Notify("NODE_REPORT", msg.str());
	m_v_past_time = m_v_current_time;
}

void Trilateration::BeaconInfo(string str)
{
	double b1_r, b2_r, b3_r;
	double distance_p2p1;
	double exx, exy, eyx, eyy;
	double i, j;
	double x, y;
	double auv_x, auv_y;
	m_b_buffer.push_back(str);
	if(m_b_buffer.size() >= 3){
		for(int k = 0; k <= 2; k++)
		{
			char id;
			string idname;
			string range;
			string beacon;
			vector<string> str_beacon;
			stringstream ss;
			beacon = m_b_buffer.front();
			m_b_buffer.pop_front();
			str_beacon = parseString(beacon, ',');
			idname = str_beacon[2];
			id = idname[idname.size()-1];
			range = str_beacon[1];
			biteStringX(range, '=');
			ss << range;
			switch(id)
			{
				case '1':
					ss >> b1_r;
					m_b1_r = b1_r;
				case '2':
					ss >> b2_r;
					m_b2_r = b2_r;
				case '3':
					ss >> b3_r;
					m_b3_r = b3_r;
			}
		}
		distance_p2p1 = sqrt(40000+90000);
		exx = -200/distance_p2p1;
		exy = -300/distance_p2p1;
		i = exx*(-600)+exy*(-200);
		eyx = (-600-i*exx)/sqrt(pow((-300-300-i*exx), 2)+pow((-100-100-i*exy), 2));
		eyy = (-200-i*exy)/sqrt(pow((-300-300-i*exx), 2)+pow((-100-100-i*exy), 2));
		j = (-600)*eyx+(-200)*eyy;
		x = (b1_r*b1_r-b2_r*b2_r+distance_p2p1*distance_p2p1)/(2*distance_p2p1);
		y = (b1_r*b1_r-b3_r*b3_r+i*i+j*j)/(2*j)-i*x/j;
		auv_x = 300+x*exx+y*eyx;
		auv_y = 100+x*exy+y*eyy;
		m_v_posx = auv_x;
		m_v_posy = auv_y;
	}
}
