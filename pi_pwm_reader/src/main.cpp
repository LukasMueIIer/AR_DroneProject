#include <wiringPi.h>
#include <sys/time.h>
#include "ros/ros.h"
#include <string>


int state; //check if 


  

struct timeval tv_RE; //holds time for rising edge
struct timeval tv_FE; //holds time for falling edge
struct timeval tv_local;
float ratio;

void InterruptBoth(void){
  int HoL = digitalRead(25);
  unsigned long int LPhase;
  unsigned long int HPhase;
  if(HoL == 1 && state){//were high and havend treated high yet
    gettimeofday(&tv_RE,NULL);
    state = !state;
  }else if(HoL == 0 && !state){
    gettimeofday(&tv_local,NULL);
    LPhase = tv_RE.tv_usec - tv_FE.tv_usec + 1000000 * (tv_RE.tv_sec - tv_FE.tv_sec);
    HPhase = tv_local.tv_usec - tv_RE.tv_usec + 1000000 * (tv_local.tv_sec - tv_RE.tv_sec);
    ratio = ((float)HPhase)/((float)LPhase);
    tv_FE = tv_local;
    state = !state;
  }
};

int main (int argc,char * argv[])
{
  ros::init(argc, argv, "pwm_reader"); //start ros
  std::string configFilePath; 
  if(!getParam ("pi_pwm_configPath",configFilePath)){
    ROS_INFO("could not find pi_pwm_configPath parameter!");
    return 0;
  }

  //pwm reading setup
  state = FALSE;
  //set initial values for rising and falling edge that arent identical
  gettimeofday(&tv_RE,NULL);
  delay(200);
  gettimeofday(&tv_FE,NULL);
  //set up pins
  wiringPiSetup () ;
  pinMode (25, INPUT) ;
  wiringPiISR (25, INT_EDGE_BOTH,  InterruptBoth); 
 

  ros::NodeHandle n;  //Handle for ROS functions

  ros::Rate loop_rate(10);

  while (ros::ok()){
    ROS_INFO("%f", ratio);
    loop_rate.sleep();
  }

  return 0 ;
}
