#include "PointAssign.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

void PointAssign::AssignByRegion()
{
	int posx, posy;
	int x;
	int posPoint, posId;
	string xx;
	while(!m_buffer.empty()){
		m_point = m_buffer.front();
		m_buffer.pop_front();
		posx = m_point.find("x=");
		posy = m_point.find(",y=");
		posId = m_point.find("id=");
		if(posx != string::npos){
			xx = m_point.substr(posx+2, posy-posx-2);
			x = atoi(xx.c_str());
			if(x <= 87){
				m_point1.push_back(m_point);
			}else{
					m_point2.push_back(m_point);
			}
		}else{
			m_point1.push_back(m_point);
			m_point2.push_back(m_point);
			if(m_point == "lastpoint"){
				m_stop = "true";
			}
		}
	}
}

void PointAssign::AssignById(){
	int posId, end;
	int id;
	string iid;
	while(!m_buffer.empty()){
	m_point = m_buffer.front();
	m_buffer.pop_front();
	posId = m_point.find("id=");
	if(posId != string::npos){
		end = m_point.size();
		iid = m_point.substr(posId+3, end-posId-2);
		id = atoi(iid.c_str());
		if(id <= 50){
			m_point1.push_back(m_point);
		}else if(id > 50 && id <= 100){
			m_point2.push_back(m_point);
		}
	}else{
		m_point1.push_back(m_point);
		m_point2.push_back(m_point);
	}
	}
}
