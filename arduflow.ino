#include <SPI.h>

#define NCS      10
#define SCK      13

#define ADNS3080_PRODUCT_ID            0x00
#define ADNS3080_MOTION                0x02
#define ADNS3080_DELTA_X               0x03
#define ADNS3080_DELTA_Y               0x04
#define ADNS3080_SQUAL                 0x05
#define ADNS3080_CONFIGURATION_BITS    0x0a
#define ADNS3080_EXTENDED_CONFIG       0x0b
#define ADNS3080_DATA_OUT_LOWER        0x0c
#define ADNS3080_DATA_OUT_UPPER        0x0d
#define ADNS3080_FRAME_CAPTURE         0x13
#define ADNS3080_FRAME_PERIOD_MAX_BOUND_LOWER      0x19
#define ADNS3080_FRAME_PERIOD_MAX_BOUND_UPPER      0x1a
#define ADNS3080_FRAME_PERIOD_MIN_BOUND_LOWER      0x1b
#define ADNS3080_FRAME_PERIOD_MIN_BOUND_UPPER      0x1c
#define ADNS3080_SHUTTER_MAX_BOUND_LOWER           0x1e
#define ADNS3080_SHUTTER_MAX_BOUND_UPPER           0x1e
#define ADNS3080_PIXEL_BURST           0x40
#define ADNS3080_MOTION_BURST          0x50
#define ADNS3080_SROM_LOAD             0x60


// read - adr, 0

unsigned char SPI_R_Reg(unsigned char reg)
{
  unsigned char stat;

  digitalWrite(NCS, 0);                   // CSN low, init SPI transaction
  delay(1);
  stat = SPI.transfer(reg);               // ..and write value to it..
  delay(1);
  stat = SPI.transfer(0);
  delay(1);
  digitalWrite(NCS, 1);                   // CSN high again

  return(stat);                  
}

unsigned char SPI_W_Reg(unsigned char reg, unsigned char value)
{
  unsigned char stat;

  digitalWrite(NCS, 0);                   // CSN low, init SPI transaction
  delay(1);
  SPI.transfer(reg | 0x80);                      // select register
  delay(1);
  stat = SPI.transfer(value);                    // ..and write value to it..
  delay(1);
  digitalWrite(NCS, 1);                   // CSN high again

  return(stat);                  
}

unsigned char pict[900];

void SPI_P_Burst()
{
  int i;

  digitalWrite(NCS, 0);                   // CSN low, init SPI transaction
  delay(1);
  SPI.transfer(ADNS3080_PIXEL_BURST);            
  delay(1);
  for (i=0;i<900;i++) pict[i] = SPI.transfer(0);
  delay(1);
  digitalWrite(NCS, 1);                   // CSN high again
}

unsigned char moti[7];

void SPI_M_Burst()
{
  int i;

  digitalWrite(NCS, 0);                   // CSN low, init SPI transaction
  delay(1);
  SPI.transfer(ADNS3080_MOTION_BURST);            
  delay(1);
  for (i=0;i<7;i++) moti[i] = SPI.transfer(0);
  delay(1);
  digitalWrite(NCS, 1);                   // CSN high again
}

unsigned char ret;

void setup()
{  
  Serial.begin(115200);
  pinMode (NCS, OUTPUT);
  digitalWrite(NCS, 1); 
  SPI.setClockDivider(SPI_CLOCK_DIV16);  
  SPI.setDataMode(SPI_MODE3);
  SPI.begin();
  delay(100);
  Serial.println("Start");
  delay(100);
  ret = SPI_R_Reg(ADNS3080_PRODUCT_ID);
  Serial.println(ret,HEX);
  if (ret == 0x17) 
  {
    Serial.println("Sensor OK");
    delay(100);
    ret = SPI_R_Reg(ADNS3080_EXTENDED_CONFIG);
    Serial.print("Ext conf ");
    Serial.println(ret,HEX);
    
    delay(100);
    ret = SPI_R_Reg(ADNS3080_CONFIGURATION_BITS);
    delay(100);
    ret |= 0x10;
    SPI_W_Reg(ADNS3080_CONFIGURATION_BITS,ret);    
  }
  else Serial.println("Sensor NOT OK");
}

void loop ()
{
  int i;
    
  // video picture
  /**/
  delay(20);
  SPI_W_Reg(ADNS3080_FRAME_CAPTURE,0x83);
  delay(5);
  SPI_P_Burst();
  Serial.write(pict,900);
  /**/
  
  // mouse data
  /*
  delay(200);
  SPI_M_Burst();
  if (moti[0] & 0x80)
  {
    Serial.print("X:");
    Serial.print(moti[1]);
    Serial.print(" Y:");
    Serial.print(moti[2]);
    Serial.print(" Q:");
    Serial.print(moti[3]);
    Serial.println("  ");
  }
  /**/
}
