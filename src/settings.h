// for <PT6311.h>
#define VFD_CS_PIN D1   //blue 
#define VFD_CLK_PIN D2  //gray 
#define VFD_DATA_PIN D3 //brown 
#define VFD_BYTES_PER_DIGIT 3 
// numbers
uint8_t f[10]={140,128,12,132,128,132,140,128,140,132};
uint8_t s[10]={196,64,195,195,71,135,135,192,199,199};
// symbols
uint8_t sf[10]={140,128,12,132,128,132,140,128,140,132};
uint8_t ss[10]={196,64,195,195,71,135,135,192,199,199};  
//
unsigned long previousMillis;
unsigned long interval;


