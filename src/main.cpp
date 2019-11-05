#include <Arduino.h>
#include <PT6311.h>
#include "settings.h"


PT6311 pt6311_driver;
uint8_t screen[10][2];


void fill_mem(uint8_t value, uint8_t number_of_bytes, uint8_t start_addr)
{
  pt6311_driver.addrSetCmd(start_addr); 
	pt6311_driver.displayMemWriteCmd(true, false);
  for (uint8_t i = 0; i < number_of_bytes; i++)
    {
        pt6311_driver.data(value, false, ((i + 1) == number_of_bytes));
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// position, lo, hi bytes, lo and hi add segments
// position from 1 to 9
// segment for attribytes from 0 to 2
void writepos(int pos,int lo, int hi, int loa, int hia){
	pt6311_driver.addrSetCmd(pos*3);
	pt6311_driver.displayMemWriteCmd(true, false);
	pt6311_driver.data(lo+loa, false, false);
	pt6311_driver.data(hi+hia, false, false);
	pt6311_driver.data(0, false, true);
}
/////////////////////////////////////////////////////////
// copy local screen array to VFD
void writearrtoscr(void){
	for (int i=0;i<10;i++){
		writepos(i,screen[i][0], screen[i][1], 0, 0);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void showtimedirect(int h, int m,int ss)
{ // выводит время прямо сразу на экран
  // seconds
  writepos(1,f[ss%10]+2, s[ss%10], 0, 0);
  writepos(2,f[(ss-ss%10)/10], s[(ss-ss%10)/10], 0, 0);
  // minutes
  writepos(3,f[m%10]+2, s[m%10], 0, 0);
  writepos(4,f[(m-m%10)/10], s[(m-m%10)/10], 0, 0);
  // hours
  writepos(5,f[h%10], s[h%10], 0, 0);
  writepos(6,f[(h-h%10)/10], s[(h-h%10)/10], 0, 0);
}
void showtime(int h, int m,int ss)
 {
  // выводит время в массив
  // seconds
  screen[1][0]=f[ss%10]+2;
  screen[1][1]=s[ss%10];
  screen[2][0]=f[(ss-ss%10)/10];
  screen[2][1]=s[(ss-ss%10)/10];
  // minutes
  screen[3][0]=f[m%10]+2;
  screen[3][1]=s[m%10];
  screen[4][0]=f[(m-m%10)/10];
  screen[4][1]=s[(m-m%10)/10];
  // hours
  screen[5][0]=f[h%10];
  screen[5][1]=s[h%10];
  screen[6][0]=f[(h-h%10)/10];
  screen[6][1]=s[(h-h%10)/10];
 }


void setup() {
  // init scrreen
  pt6311_driver.init(VFD_CS_PIN, VFD_CLK_PIN, VFD_DATA_PIN);
  pt6311_driver.reset(VFD_DISP_MODE_10D18S); // good VFD_DISP_MODE_10D18S
   for (uint8_t i = 0; i < 12; i++) 
    {
      fill_mem(0x00, VFD_BYTES_PER_DIGIT, i * VFD_BYTES_PER_DIGIT);
      //delay(10); 
    }
  // end init screen

}

void loop() {
  
  

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval)
  {
   second=second+1;
   second==60?second=0:second=second;
   previousMillis = currentMillis;

   showtime(0,0,0);
   writearrtoscr();
   }
}