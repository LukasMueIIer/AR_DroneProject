#include <wiringPi.h>
#include <stdio.h>

int main (void)
{
  int iPinStatus = 0;
  wiringPiSetup () ;
  pinMode (25, INPUT) ;
  for (;;)
  {
    iPinStatus = digitalRead(25);
    cout("%d \n",iPinStatus);

  }
  return 0 ;
}