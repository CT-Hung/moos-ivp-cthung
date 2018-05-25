/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_ZigLeg.h"
#include "ZAIC_PEAK.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_ZigLeg::BHV_ZigLeg(IvPDomain domain) :
  IvPBehavior(domain)
{
  m_osx = 0.0;
  m_osy = 0.0;
  m_angle = 45.0;
  m_pulse_duration = 5.0;
  m_curr_time = 0.0;
  m_index_time = 0.0;
  m_index = 0.0;
  m_index_check = 0.0;
  m_change_angle = 0.0;
  m_in = 1;
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("WPT_INDEX");
  addInfoVars("NAV_HEADING");
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_ZigLeg::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if(param == "pulse_duration") {
    m_pulse_duration = double_val;
    return(true);
  }
  else if (param == "pulse_angle") {
    m_angle = double_val;
    return(true);
  }

  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_ZigLeg::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_ZigLeg::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_ZigLeg::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_ZigLeg::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_ZigLeg::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_ZigLeg::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_ZigLeg::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_ZigLeg::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;
  bool ok1, ok2, ok3;
  double distance;
  double change_angle;
  m_osx = getBufferDoubleVal("NAV_X", ok1);
  m_osy = getBufferDoubleVal("NAV_Y", ok2);
  m_curr_time = getBufferCurrTime();
  m_index = getBufferTimeVal("WPT_INDEX");
  if(m_index == 0){
    m_index_time = getBufferCurrTime();
    m_in = 0;
  }
  postMessage("HEADING", m_osh);
  distance = m_curr_time-m_index_time;
  if(m_change_angle >= 360){
    m_change_angle -= 360;
  }
  if(!ok1 || !ok2){
    postWMessage("No ownship X/Y info in info_buffer");
    return(0);
  }else if(distance > 4 && distance <5){
    m_osh = getBufferDoubleVal("NAV_HEADING", ok3);
    m_change_angle = m_angle+m_osh;
  }else if(m_in == 0 && distance >= 5){
    ipf = buildFunctionWithZAIC();
    if(distance >= (m_pulse_duration+5)){
      m_in = 1;
    }
  }


  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

IvPFunction *BHV_ZigLeg::buildFunctionWithZAIC()
{
  ZAIC_PEAK crs_zaic(m_domain, "course");
  crs_zaic.setSummit(m_change_angle);
  crs_zaic.setPeakWidth(0);
  crs_zaic.setBaseWidth(0);
  IvPFunction *ivp_function = 0;
  ivp_function = crs_zaic.extractIvPFunction();
  return(ivp_function);
}

