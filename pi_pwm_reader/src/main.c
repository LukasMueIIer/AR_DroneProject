#include <wiringPi.h>
#include <stdio.h>

void testInterrupt(void);

int main (void)
{
  int iPinStatus = 0;
  wiringPiSetup () ;
  pinMode (25, INPUT) ;

  wiringPiISR (25, INT_EDGE_RISING,  testInterrupt); 
  // for (;;)
  // {
  //   iPinStatus = digitalRead(25);
  //   print("%d \n",iPinStatus);

  // }
  for(;;){

  }


  return 0 ;
}

void testInterrupt(void){
  printf("Interrupt \n");
}