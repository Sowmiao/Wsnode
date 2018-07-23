#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

SoftwareSerial mySerial(rxPin,txPin);
unsigned char Buff[10]={0x00,0x4,0x00,0x00,0x00,0x00,0xff,0xcb,0x00,0x00};
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
pinMode(rxPin, INPUT);
pinMode(txPin, OUTPUT);
  unsigned short calcCrc16, var;
  mySerial.begin(9600);
  Serial.begin(9600);
  calcCrc16 = crc16(Buff, UART_FRAME_HDR_HDR_CRC_FIELD_OFF);
  var = calcCrc16;
  //Buff[6] = (var >> 8);
  //Buff[7] = (var & 0xff);
  //Buff[8] = 0;
  //Buff [9] = 0;

  
   htons(Buff, 0x34);
   //nsigned short crc16(unsigned char *buff_p, unsigned int len);

   int i;
  while(i<10)
   {
    mySerial.write(Buff[i]);
    //Serial.println(Buff[i],HEX);
    i++;
   }
}

  int id =0;
void loop() {

   
  while(mySerial.available())
  {
    int inByte = mySerial.read();
    //Serial.print(id++);
    Serial.println(inByte,HEX);
  }
  
}

void htons(unsigned char *buff_p, unsigned short val)
{
  buff_p[0]=(val >> 8) & 0xff;
  buff_p[1]= (val) & 0xff;
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

