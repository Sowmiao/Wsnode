#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

SoftwareSerial mySerial(rxPin,txPin);
unsigned char Buff[8]={0xfc,0xc2,0x3d,0x00,0x00,0x10, 0x25, 0xEB};
#define UART_FRAME_HDR_MSG_TYPE_FIELD_LEN   2 
#define UART_FRAME_HDR_FLAGS_FIELD_LEN      1 
#define UART_FRAME_HDR_SEQ_NR_FIELD_LEN     1 
#define UART_FRAME_HDR_PYLD_LEN_FIELD_LEN   2 
#define UART_FRAME_HDR_CRC_FIELD_LEN        2 
#define UART_FRAME_HDR_MSG_TYPE_FIELD_OFF   0 

#define UART_FRAME_HDR_FLAGS_FIELD_OFF UART_FRAME_HDR_MSG_TYPE_FIELD_LEN 
 
#define UART_FRAME_HDR_SEQ_NR_FIELD_OFF (UART_FRAME_HDR_FLAGS_FIELD_OFF + UART_FRAME_HDR_FLAGS_FIELD_LEN) 
 
#define UART_FRAME_HDR_PYLD_LEN_FIELD_OFF (UART_FRAME_HDR_SEQ_NR_FIELD_OFF + UART_FRAME_HDR_SEQ_NR_FIELD_LEN) 
 
#define UART_FRAME_HDR_HDR_CRC_FIELD_OFF (UART_FRAME_HDR_PYLD_LEN_FIELD_OFF + UART_FRAME_HDR_PYLD_LEN_FIELD_LEN) 
 
#define UART_FRAME_HDR_PYLD_CRC_FIELD_OFF (UART_FRAME_HDR_HDR_CRC_FIELD_OFF  + UART_FRAME_HDR_CRC_FIELD_LEN) 
 
 
#define UART_FRAME_HDR_LEN (UART_FRAME_HDR_MSG_TYPE_FIELD_LEN + UART_FRAME_HDR_FLAGS_FIELD_LEN+ UART_FRAME_HDR_SEQ_NR_FIELD_LEN + UART_FRAME_HDR_PYLD_LEN_FIELD_LEN + UART_FRAME_HDR_HDR_CRC_FIELD_LEN + UART_FRAME_HDR_PYLD_CRC_FIELD_LEN) 


void setup() 
{ 
unsigned char *buff_p;
pinMode(rxPin, INPUT);
pinMode(txPin, OUTPUT);
  unsigned short calcCrc16, var;
  mySerial.begin(9600);
  Serial.begin(9600);
   calcCrc16 = crc16(Buff, UART_FRAME_HDR_PYLD_CRC_FIELD_OFF);
   //htons(buff_p, calcCrc16);
   var = calcCrc16;
  Buff[8] = (var >> 8);
  Buff[9] = (var & 0xff);
  //Buff[8] = 0;
  
  Serial.println(var,HEX);
  //Serial.println(,HEX);
}

void htons(unsigned char *buff_p, unsigned short val)
{
   buff_p[0] = (val >> 8) & 0xff;
   buff_p[1] = (val) & 0xff;
}
unsigned short crc16(unsigned char *buff_p, unsigned int len)
{
   unsigned long ckSum = 0;

   while (len > 1)
   {
      unsigned short temp = *buff_p;
      temp = (temp << 8) | (*(buff_p + 1));
      ckSum = ckSum + temp;
      buff_p += 2;
      len -= 2;
   }

   if (len > 0)
       ckSum += (*buff_p);

   while (ckSum >> 16)
   {
      ckSum = (ckSum & 0xffff) + (ckSum >> 16);
   }
    return (unsigned short)(~ckSum);
}
void loop()
{

}


