///////////////////////////////////////////////////////////////////////////
//
//  Description:  
//      Flight computer interface
//
//  Comments:
//
//  Abitamim bharmal
//  Olin College of Engineering
//  2/11/2022
///////////////////////////////////////////////////////////////////////////   


#ifndef rev_1_functions_h
#define rev_1_functions_h

//comment this line for cpp testing but uncomment for arduino
//#include <Arduino.h>





double[] gps();
int radio(telemetry_point tp);
double altimeter();
double global_z(double[] axis);
double[] imu();



#endif