#include <PT6311.h>

#define VFD_CS_PIN D1   //blue 
#define VFD_CLK_PIN D2  //gray 
#define VFD_DATA_PIN D3 //brown 

#define VFD_BYTES_PER_DIGIT 3      //3
PT6311 pt6311_driver;
// numbers
int f[10]={140,128,12,132,128,132,140,128,140,132};
int s[10]={196,64,195,195,71,135,135,192,199,199};
// symbols
int sf[10]={140,128,12,132,128,132,140,128,140,132};
int ss[10]={196,64,195,195,71,135,135,192,199,199};

void fill_mem(uint8_t value, uint8_t number_of_bytes, uint8_t start_addr)
{
  pt6311_driver.addrSetCmd(start_addr); 
	pt6311_driver.displayMemWriteCmd(true, false);
  for (uint8_t i = 0; i < number_of_bytes; i++)
    {
        pt6311_driver.data(value, false, ((i + 1) == number_of_bytes));
    }
}

void showfirst(int s1,int s2, int s3)
{
  //27
  pt6311_driver.addrSetCmd(27);
  pt6311_driver.displayMemWriteCmd(true, false);
  pt6311_driver.data(sf[s1]+0, false, false);
  pt6311_driver.data(ss[s1], false, false);
  pt6311_driver.data(0, false, true);
  //24
  pt6311_driver.addrSetCmd(24);
  pt6311_driver.displayMemWriteCmd(true, false);
  pt6311_driver.data(sf[s2]+2, false, false);
  pt6311_driver.data(ss[s2], false, false);
  pt6311_driver.data(0, false, true);
  //21
  pt6311_driver.addrSetCmd(21);
  pt6311_driver.displayMemWriteCmd(true, false);
  pt6311_driver.data(sf[s3]+0, false, false);
  pt6311_driver.data(ss[s3], false, false);
  pt6311_driver.data(0, false, true);  
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
uint8_t[10][2] screen;
void writearr(void){
	for (int i=0;i<10;i++){
		writepos(i,screen[i][0], screen[i][1], 0, 0);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void showtime(int h, int m,int ss)
{
  //18
  pt6311_driver.addrSetCmd(18);
  pt6311_driver.displayMemWriteCmd(true, false);
  pt6311_driver.data(f[(h-h%10)/10]+0, false, false);
  pt6311_driver.data(s[(h-h%10)/10], false, false);
  pt6311_driver.data(0, false, true);
  //15
  pt6311_driver.addrSetCmd(15);
  pt6311_driver.displayMemWriteCmd(true, false);
  pt6311_driver.data(f[h%10]+2, false, false);
  pt6311_driver.data(s[h%10], false, false);
  pt6311_driver.data(0, false, true);
  //12
  pt6311_driver.addrSetCmd(12);
  pt6311_driver.displayMemWriteCmd(true, false);
  pt6311_driver.data(f[(m-m%10)/10]+0, false, false);
  pt6311_driver.data(s[(m-m%10)/10], false, false);
  pt6311_driver.data(0, false, true);
  //9
  pt6311_driver.addrSetCmd(9);
  pt6311_driver.displayMemWriteCmd(true, false);
  pt6311_driver.data(f[m%10]+2, false, false);
  pt6311_driver.data(s[m%10], false, false);
  pt6311_driver.data(0, false, true);
  //6
  pt6311_driver.addrSetCmd(6);
  pt6311_driver.displayMemWriteCmd(true, false);
  pt6311_driver.data(f[(ss-ss%10)/10]+0, false, false);
  pt6311_driver.data(s[(ss-ss%10)/10], false, false);
  pt6311_driver.data(0, false, true);
  //3
  pt6311_driver.addrSetCmd(3);
  pt6311_driver.displayMemWriteCmd(true, false);
  pt6311_driver.data(f[ss%10], false, false);
  pt6311_driver.data(s[ss%10], false, false);
  pt6311_driver.data(0, false, true);
}

void setup(void){
 //----------------Display INIT
  pt6311_driver.init(VFD_CS_PIN, VFD_CLK_PIN, VFD_DATA_PIN);
  pt6311_driver.reset(VFD_DISP_MODE_10D18S); // good VFD_DISP_MODE_10D18S
   for (uint8_t i = 0; i < 12; i++) 
  {
      fill_mem(0x00, VFD_BYTES_PER_DIGIT, i * VFD_BYTES_PER_DIGIT);
      delay(10); 
  }
  showtime(0,0,0);
  //----------------Display
}

void loop(void){
  server.handleClient();
  if ((second == 0)||(second == 1)){            // если насчитали 60 сек
    synctime();                  // синхронизируем время
  }
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval)
  {
   second=second+1;
   second==60?second=0:second=second;
   previousMillis = currentMillis;
   Serial.print(hour);
   Serial.print(":");
   Serial.print(minute);
   Serial.print(":");
   Serial.print(second);
   Serial.println(".");
   showtime(hour,minute,second);
   }
}

