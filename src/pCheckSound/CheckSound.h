/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: CheckSound.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef CheckSound_HEADER
#define CheckSound_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
#include <vector>

class CheckSound : public AppCastingMOOSApp
{
 public:
   CheckSound();
   ~CheckSound();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void ChangeToPressure();
   void CalSEL();

 protected:
   void registerVariables();
 protected:
   int m_SampleRate;
   double m_SEL;
   double m_sen;
   short m_real_time_SEL;
   short m_real_time_SPLrms;
   short m_max_SEL;
   short m_max_SPLrms;
   bool m_stop;
   bool m_start;
   float m_SEL_time;
   int m_bits;
   std::deque<std::string> m_input_buffer;
   std::deque<double> m_pressure_buffer;
};

#endif 
