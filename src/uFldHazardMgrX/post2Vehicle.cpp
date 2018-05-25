
#include "HazardMgr.h"

using namespace std;

void HazardMgr::postFinishMail()
{
	string node_mess;
	node_mess = "src_node="+m_host_community+",dest_node="+m_cname+",var_name=START_POST,string_val=true";
	Notify("NODE_MESSAGE_LOCAL", node_mess);
}

void HazardMgr::postGotFinishMail()
{
	string node_mess;
	node_mess = "src_node="+m_host_community+",dest_node="+m_cname+",var_name=GOT_FINISH,string_val=true";
	Notify("NODE_MESSAGE_LOCAL", node_mess);
}

void HazardMgr::postCollMail()
{
	Notify("NODE_MESSAGE_LOCAL", m_node_mess);
}
