/************************************************************/
/*    NAME: CTHung                                              */
/*    ORGN: MIT                                             */
/*    FILE: BearingAngle.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef BearingAngle_HEADER
#define BearingAngle_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <eigen3/unsupported/Eigen/NonLinearOptimization>

class BearingAngle : public AppCastingMOOSApp
{
 public:
   BearingAngle();
   ~BearingAngle();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   void registerVariables();

 protected:
   void micUpdate();
   void TDOA();
   void Bearing2Mic(double, double);

 private: // Configuration variables

 private: // State variables
   // enviroment 
   double m_c; //sound velocity

   //boat status 
   double m_b_x, m_b_y;
   double m_b_h;
   double m_b_m; //distance between center and mic

   //mic status 
   double m_m1_x, m_m1_y;
   double m_m1_h;
   double m_m1_rt; //receive time
   double m_m2_x, m_m2_y;
   double m_m2_h;
   double m_m2_rt; //receive time
   double m_m3_x, m_m3_y;
   double m_m3_h;
   double m_m3_rt; //receive time
   double m_m1_m2_dl; //distance between m1 and m2 

   //source status
   double m_source_guessAngle;
};

#endif 
