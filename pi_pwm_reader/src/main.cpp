#include <wiringPi.h>
#include <sys/time.h>
#include "ros/ros.h"
#include <string>
#include "std_msgs/Float64.h"
#include <functional>

#define pinAmount 8   //this holds the max amount of pins that can be used
                      //doesnt indicate the actuall amount but determines memory reserved
int actualPins;       //actuall amount of pins that are used
int InteruptToPin[8]; //holds the Info what Interrupt belongs to what pin

int state[pinAmount]; //this array holds the information if the last interrupt on the pin was a 
                      //high or low     0: LOW;  1: HIGH;

struct timeval tv_RE[pinAmount]; //holds time for rising edge
struct timeval tv_FE[pinAmount]; //holds time for falling edge
struct timeval tv_local[pinAmount]; //placeholder to have reserved memory for 
                                    //working with a local time in interrupts should prevent exceptions

float ratio[pinAmount];             //ratio of high to low phase on pin to calc pwm value

ros::Publisher pwm_pub[pinAmount];  //publishers for publishing on ros topics

void InterruptBoth(int iInterrupt){
  int HoL = digitalRead(InteruptToPin[iInterrupt]); //get Pin belongign to interrupt and check its state
  
  unsigned long int LPhase; //hold the low and high phase duration for ratio calculation
  unsigned long int HPhase;

  //check if pin state has change if so act according to new state
  if(HoL == 1 && state[iInterrupt]){//were high and havent treated high yet

    gettimeofday(&tv_RE[iInterrupt],NULL);      //save current time
    state[iInterrupt] = !state[iInterrupt];                 //and flip state

  }else if(HoL == 0 && !state[iInterrupt]){//were low and havent treated low yet

    gettimeofday(&tv_local[iInterrupt],NULL); //save current time in local var

    LPhase = tv_RE[iInterrupt].tv_usec - tv_FE[iInterrupt].tv_usec + 1000000 * (tv_RE[iInterrupt].tv_sec - tv_FE[iInterrupt].tv_sec);
    HPhase = tv_local[iInterrupt].tv_usec - tv_RE[iInterrupt].tv_usec + 1000000 * (tv_local[iInterrupt].tv_sec - tv_RE[iInterrupt].tv_sec);
    ratio[iInterrupt] = ((float)HPhase)/((float)LPhase);
    tv_FE[iInterrupt] = tv_local[iInterrupt];
    state[iInterrupt] = !state[iInterrupt];
  }
};



int main (int argc,char * argv[])
{
  //ROS SETUP
  ros::init(argc, argv, "pwm_reader"); //start ros
  ros::NodeHandle n;  //Handle for ROS functions
  wiringPiSetup();    //start wiringPi
  ros::Rate loop_rate(10);  //for message publishing rate

  //LOAD CONFIG FROM FILE
  {
    std::string configFilePath;           
    if(! n.getParam ("pi_pwm_configPath",configFilePath)){
      ROS_INFO("could not find pi_pwm_configPath parameter!");
      return 0;
    }
    //dummy load function not implemented yet
    actualPins = 2; //two actual pins

    InteruptToPin[0] = 25;  //save PIN ID
    InteruptToPin[1] = 24;
  }
  
  //SET UP ROS TOPICS
  {
    std::string ls; 
    for(int i = 0; i < actualPins; ++i){  //loop over interrupts to be set
      ls = "PWM_";
      ls.append(std::to_string(i));
      pwm_pub[i] = n.advertise<std_msgs::Float64>(ls, 1000);  //create topic
      state[i] = FALSE; //set a initial state

      //set initial times that arent identical to avoid excetions
      gettimeofday(&tv_RE[i],NULL);
      delay(100);
      gettimeofday(&tv_FE[i],NULL);
     
      pinMode(InteruptToPin[i], INPUT); //set Interupt Pin to read
       std::function<void()> InterFunc = [i](){InterruptBoth(i);};
      wiringPiISR (InteruptToPin[i], INT_EDGE_BOTH, *InterFunc.target<void(*)()>()); //set Interrupt Function
    }

  }

  
  
  std_msgs::Float64 msg;

  while (ros::ok()){
    for(int i = 0; i < actualPins; ++i){ //loop through all ratios and publish
      msg.data = ratio[i];
      pwm_pub[i].publish(msg);
    }
    
    loop_rate.sleep(); //sleep for rate
  }

  return 0 ;
}

