#include "GenPath.h"
#include <stdlib.h>
#include <sstream>
#include <math.h>
using namespace std;

void GenPath::GetXY(){
	int posx, posy, posid;
	int x, y;
	while(!m_buffer.empty()){
		m_point = m_buffer.front();
		m_buffer.pop_front();
		posx = m_point.find("x=");
		posy = m_point.find(",y=");
		posid = m_point.find(",id=");
		if(posx != string::npos && posy != string::npos){
			x = atoi(m_point.substr(posx+2, posy-posx-2).c_str());
			y = atoi(m_point.substr(posy+3, posid-posy-3).c_str());
			if(m_tx != x || m_ty != y){
				m_seglist.add_vertex(x, y);
				stringstream s;
				s << "x=" << x << ",y=" << y;
				m_check.push_back(s.str());
				m_tx = x;
				m_ty = y;
				m_c++;
				m_rece++;
			}
		}else{
			m_in++;
		}
	}
}

void GenPath::CheckPoint(){

	string point;
	int posx, posy;
	int x, y;
	double dx, dy, d;
	int r;
	point = m_check.at(m_index);
	posx = point.find("x=");
	posy = point.find(",y=");
	r = atoi(m_r.c_str());
	if(posx != string::npos){
		x = atoi(point.substr(posx+2, posy-posx-2).c_str());
		y = atoi(point.substr(posy+3, point.size()).c_str());
		dx = x-m_x;
		dy = y-m_y;
		d = sqrt(dx*dx+dy*dy);
		if(d <= r){
			m_check.at(m_index) = "";
			m_c--;
			m_visit++;
		}
	}
}


void GenPath::GetGen(){
	int posx, posy, posend;
	int x, y;
	for(int i = 0; i < m_check.size(); i++){
		if(m_check.at(i) != ""){
			m_point = m_check.at(i);
			posx = m_point.find("x=");
			posy = m_point.find(",y=");
			posend = m_point.find("/n");
			if(posx != string::npos && posy != string::npos){
				x = atoi(m_point.substr(posx+2, posy-posx-2).c_str());
				y = atoi(m_point.substr(posy+3, posend-posy-3).c_str());
				m_seglist.add_vertex(x, y);
				m_buffer1.push_back(m_check.at(i));
			}
		}
	}
	m_check.clear();
	for(int i = 0; i < m_buffer1.size(); i++){
		m_check.push_back(m_buffer1.at(i));
	}
	m_buffer1.clear();
}
