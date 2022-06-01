#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>

void InterruptRE(void);

void InterruptFE(void);

unsigned long int val;

struct timeval tv_RE; //holds time for rising edge
struct timeval tv_FE; //holds time for falling edge

int main (void)
{


  //set initial values for rising and falling edge that arent identical
  gettimeofday(&tv_RE,NULL);
  delay(200);
  gettimeofday(&tv_FE,NULL);

  //set up pins
  wiringPiSetup () ;
  pinMode (25, INPUT) ;

  wiringPiISR (25, INT_EDGE_RISING,  InterruptRE); 
  //wiringPiISR (25, INT_EDGE_FALLING,  InterruptFE); 
  // for (;;)
  // {
  //   iPinStatus = digitalRead(25);
  //   print("%d \n",iPinStatus);

  // }
  for(;;){
    printf("%ld \n",val);
  }


  return 0 ;
}

void InterruptFE(void){
  gettimeofday(&tv_FE,NULL);
  wiringPiISR (25, INT_EDGE_RISING,  InterruptRE);
}

void InterruptRE(void){
  struct timeval tv_local; //local time for rising edge, global for permanent memory place

  //seconds and ms for High/Low
  unsigned long int iSecDiffHigh;
  unsigned long int iMSDiffHigh;
  unsigned long int iSecDiffLow;
  unsigned long int iMSDiffLow;
  float quote; //Quot for high and low time
  
  gettimeofday(&tv_local,NULL); //get time of interrupt

  iSecDiffHigh = tv_FE.tv_sec - tv_RE.tv_sec;
  iMSDiffHigh = tv_FE.tv_usec - tv_RE.tv_usec + 1000000 * iSecDiffHigh;
  
  iSecDiffLow = tv_local.tv_sec - tv_FE.tv_sec;
  iMSDiffLow = tv_local.tv_usec - tv_FE.tv_usec + 1000000 * iSecDiffLow;

  val = iMSDiffLow;

  quote = ((float)iMSDiffHigh)/((float)iMSDiffLow);
  

  tv_RE = tv_local;
  wiringPiISR (25, INT_EDGE_FALLING,  InterruptFE);

}