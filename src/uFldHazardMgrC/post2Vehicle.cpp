#include "MBUtils.h"
#include "HazardMgr.h"
#include "NodeMessage.h"
#include "NodeMessageUtils.h"
#include "ACTable.h"
#include <iostream>
using namespace std;

void HazardMgr::gotCollMail()
{
	string msg;
	NodeMessage node_msg;
	node_msg.setSourceNode(m_host_community);
	node_msg.setDestNode(m_cname);
	node_msg.setVarName("GOT_COLL");
	node_msg.setStringVal("true");
	msg = node_msg.getSpec();

	Notify("NODE_MESSAGE_LOCAL", msg);
}

void HazardMgr::mergePoint(string str)
{
	int pos1, pos2, posc;
	string point1, point2;
	string reportPath;
	point1 = str;
	point2 = m_pt;
	if(point1 == "" && point2 == ""){
		string msg;
		msg = "Detect no thing";
		reportEvent(msg);
	}else if(str != m_test){
		if(point2 == ""){
			reportPath = "points={"+point1+"}";
		}else if(point1 == ""){
			reportPath = "points={"+point2+"}";
		}else{
			reportPath = "points={"+point2+":"+point1+"}";
			m_test = str;
			reportEvent(reportPath);
			m_restart_point = reportPath;
	//	Notify("HAZARDSET_POINT", reportPath);
		}
	}
}

void HazardMgr::pointPath()
{
	string str;
	string pt;
	string node_mess;
	XYSegList seglist;
	seglist.clear();
	str = m_report;
	do{
		int posx, posy, posl;
		string point;
		int x, y;
		posx = str.find("x=");
		posy = str.find(",y=");
		posl = str.find(",label=");
		if(posx == string::npos || posy == string::npos){
			break;
		}else{
			x = atoi(str.substr(posx+2, posy-posx-2).c_str());
			y = atoi(str.substr(posy+3, posl-posy-3).c_str());
			str.erase(0, posl+6);
			seglist.add_vertex(x, y);
		}
	}while(1);
	pt = seglist.get_spec();
	pt.erase(0, 5);
	pt = pt.substr(0, pt.size()-1);
	m_pt = pt;
	reportEvent(m_pt);
	NodeMessage node_msg;
	node_msg.setSourceNode(m_host_community);
	node_msg.setDestNode(m_cname);
	node_msg.setVarName("COLL_REPORT");
	node_msg.setStringVal(pt);
	m_point_path = node_msg.getSpec();

}

void HazardMgr::postFinalReport()
{
	string node_mess;
	string msg;
	NodeMessage node_msg;
	int posx, posy, posl;
	posx = m_restart_sep.find("#x=");
	posy = m_restart_sep.find(",y=");
	posl = m_restart_sep.find(",label=");
	if(posx == string::npos && posy == string::npos && posl == string::npos){
		reportEvent("Not yet");
	}else{
		m_restart_post = "false";
//		m_restart_sep = m_restart_sep.substr(posx, m_restart_sep.size());
		if(m_restart_sep.size() >= 30){
			msg = m_restart_sep.substr(0, 30);
     	m_restart_sep.erase(0, 30);
     	node_msg.setSourceNode(m_host_community);
     	node_msg.setDestNode(m_cname);
     	node_msg.setVarName("FINAL_REPORT");
     	node_msg.setStringVal(msg);
     	node_mess = node_msg.getSpec();
     	Notify("NODE_MESSAGE_LOCAL", node_mess);
		 	reportEvent("part of send");
		 ostringstream oss;
		 oss << node_mess.size();
		reportEvent(oss.str());
   }else{
     node_msg.setSourceNode(m_host_community);
     node_msg.setDestNode(m_cname);
     node_msg.setVarName("FINAL_REPORT");
     node_msg.setStringVal(m_restart_sep);
     node_mess = node_msg.getSpec();
     Notify("NODE_MESSAGE_LOCAL", node_mess);
		 m_restart_final = "false";
		 reportEvent(node_mess);
		 reportEvent("Complete send");
		 ostringstream oss;
		 oss << node_mess.size();
		reportEvent(oss.str());
		}
	}
}

void HazardMgr::mergeReport(string str){

 //  if(str.size() >= 60){
  //   m_final_report = m_final_report+str;
	//	 reportEvent("get report");
	//	 reportEvent("str");
	//	 ostringstream oss;
	//	 oss << str.size();
	//	reportEvent(oss.str());
//   }else{
     m_final_report = m_final_report+str;
     Notify("HAZARDSET_REPORT", m_final_report);
		 reportEvent("got report");
  // }                                      

}
void  HazardMgr::postRePost(){
	string msg;
	NodeMessage node_msg;
	node_msg.setSourceNode(m_host_community);
	node_msg.setDestNode(m_cname);
	node_msg.setVarName("MERGE_TIME");
	node_msg.setStringVal("true");
	msg = node_msg.getSpec();

	Notify("NODE_MESSAGE_LOCAL", msg);
}



