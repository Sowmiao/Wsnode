
#include <Arduino.h>

using namespace std;

#define SER_BUFF_LEN 1024
unsigned char serTxBuff[SER_BUFF_LEN];
unsigned char serRxBuff[SER_BUFF_LEN];

#define SHIELD_DATA_CNFRM_STS_FIELD_LEN  1
#define SHIELD_MSG_TYPE_NWK_JOIN_IND     1
#define SHIELD_MSG_TYPE_NODE_REBOOT_IND  2
#define SHIELD_MSG_TYPE_EXT_APP_DATA_CNFRM     3
#define SHIELD_MSG_TYPE_EXT_APP_DATA_IND 4
#define SHIELD_MSG_TYPE_NWK_SCAN_FLR_IND 5
#define SHIELD_MSG_TYPE_NODE_INIT_DONE   6
#define SHIELD_MSG_TYPE_NODE_EXT_ADDR_INFO     7
#define SHIELD_MSG_TYPE_RF_BAUD_RATE_INFO 8
#define SHIELD_MSG_TYPE_DATA_PUSH_INTERVAL_INFO 9

#define SHIELD_MSG_TYPE_NODE_RESET_REQ    50 
#define SHIELD_MSG_TYPE_EXT_APP_DATA_REQ  51
#define SHIELD_MSG_TYPE_GET_NODE_EXT_ADDR_INFO_REQ 52
#define SHIELD_MSG_TYPE_GET_RF_BAUD_RATE_INFO_REQ  53
#define SHIELD_MSG_TYPE_GET_DATA_PUSH_INTERVAL_INFO_REQ 54
#define SHIELD_MSG_TYPE_START_NODE_REQ 55
  
#define SHIELD_MSG_TYPE_ANY 255

#define LPWMN_MAC_SHORT_ADDR_LEN  2
#define LPWMN_MAC_EXT_ADDR_LEN  8

#define UART_MSG_TYPE_ACK   0x00

#define UART_MSG_TYPE_STREAM_TX_STS 0x08
#define UART_HDR_ACK_BM (1<<7)
#define UART_PYLD_ACK_BM (~(1<<7))
#define UART_ACK_STS_OK_BM 1 //(1<<0)
#define UART_ACK_STS_OOM_BM 2 //(1<<1)
#define UART_ACK_STS_FRAME_TOO_LONG_BM 4
#define UART_ACK_STS_INV_CRC 8
#define UART_ACK_STS_RELAY_IN_PROGRESS 16
#define UART_ACK_STS_HDR_BYTES_MISSING 32
#define UART_ACK_STS_PYLD_BYTES_MISSING 64
#define UART_FRAME_HDR_MSG_TYPE_FIELD_LEN 2
#define UART_FRAME_HDR_FLAGS_FIELD_LEN 1
#define UART_FRAME_HDR_SEQ_NR_FIELD_LEN 1
#define UART_FRAME_HDR_PYLD_LEN_FIELD_LEN 2
#define UART_FRAME_HDR_CRC_FIELD_LEN 2
#define UART_FRAME_HDR_MSG_TYPE_FIELD_OFF 0
#define UART_FRAME_MAX_PYLD_LEN 128

#define UART_FRAME_HDR_HDR_CRC_FIELD_LEN   (UART_FRAME_HDR_CRC_FIELD_LEN)
#define UART_FRAME_HDR_PYLD_CRC_FIELD_LEN  (UART_FRAME_HDR_CRC_FIELD_LEN)
#define UART_FRAME_HDR_FLAGS_FIELD_OFF UART_FRAME_HDR_MSG_TYPE_FIELD_LEN

#define UART_FRAME_HDR_SEQ_NR_FIELD_OFF UART_FRAME_HDR_FLAGS_FIELD_OFF+UART_FRAME_HDR_FLAGS_FIELD_LEN
#define UART_FRAME_HDR_PYLD_LEN_FIELD_OFF UART_FRAME_HDR_SEQ_NR_FIELD_OFF+UART_FRAME_HDR_SEQ_NR_FIELD_LEN
#define UART_FRAME_HDR_HDR_CRC_FIELD_OFF UART_FRAME_HDR_PYLD_LEN_FIELD_OFF+UART_FRAME_HDR_PYLD_LEN_FIELD_LEN
#define UART_FRAME_HDR_PYLD_CRC_FIELD_OFF UART_FRAME_HDR_HDR_CRC_FIELD_OFF+UART_FRAME_HDR_HDR_CRC_FIELD_LEN
#define UART_FRAME_HDR_LEN (UART_FRAME_HDR_MSG_TYPE_FIELD_LEN + UART_FRAME_HDR_FLAGS_FIELD_LEN + UART_FRAME_HDR_SEQ_NR_FIELD_LEN + UART_FRAME_HDR_PYLD_LEN_FIELD_LEN + UART_FRAME_HDR_HDR_CRC_FIELD_LEN + UART_FRAME_HDR_PYLD_CRC_FIELD_LEN)

unsigned char dataCnfrmSts = 0;
int hdrFlags = 0;
int hdrAckFlr = 0;

class External_address_info 
{
  public: 
  int buildSendHdr(int , unsigned char * , int );
  //void print_usage();
  unsigned short crc16(unsigned char * , unsigned int );
  //int processRcvdMsg(unsigned char *,int,int,int);
  unsigned short htons(unsigned char * , unsigned short );
  void sendNodeExtAddrInfoReq(void) 
  {
      buildSendHdr(SHIELD_MSG_TYPE_GET_NODE_EXT_ADDR_INFO_REQ, NULL, 0);
   }
 void sendRFBaudRateInfoReq(void)
    {
      buildSendHdr(SHIELD_MSG_TYPE_GET_RF_BAUD_RATE_INFO_REQ, NULL, 0);
    }  
    
 void sendDPIInfoReq(void)
 {
  buildSendHdr(SHIELD_MSG_TYPE_GET_DATA_PUSH_INTERVAL_INFO_REQ, NULL, 0);
 }

 void sendNodeStartReq(void)
 {
  buildSendHdr(SHIELD_MSG_TYPE_START_NODE_REQ, NULL, 0);
 }

 void sendNodeInitDone(void)
 {
  buildSendHdr(SHIELD_MSG_TYPE_NODE_INIT_DONE,NULL,0);
 }

 void sendNwkjoininitialization(void)
 {
  buildSendHdr(SHIELD_MSG_TYPE_NWK_JOIN_IND,NULL,0);
 }

 void sendNwkScanInit(void)
 {
  buildSendHdr(SHIELD_MSG_TYPE_NWK_SCAN_FLR_IND,NULL,0);
 }
 

 /**int readSerIntf(int expMsgType)
 {

        Serial.println ("ok");
        int off = 0;
        int readLen, pyldLen;
        int totReadLen = 0;
        int currMsgType = 0xffff, done = 0;
        int hdrAckFlr = 0;
        readLen = UART_FRAME_HDR_PYLD_CRC_FIELD_OFF;
        //memcpy(serRxBuff, 0, sizeof(serRxBuff));
        Serial.println (off);
        Serial.println (readLen);
        Serial.println (totReadLen,HEX);
        totReadLen += readLen;
        off += readLen;
        Serial.println(totReadLen,HEX);
        switch (totReadLen) {
        case UART_FRAME_HDR_PYLD_CRC_FIELD_OFF:
        {
            int idx;
            unsigned short calcCrc16;
            unsigned short rxdCrc16;
            Serial.println (readLen);
            for (idx = 0; idx < totReadLen; idx++) {
            Serial.println (serRxBuff[idx],HEX);
            }
            calcCrc16 = crc16(serRxBuff, UART_FRAME_HDR_HDR_CRC_FIELD_OFF);
            rxdCrc16 = serRxBuff[UART_FRAME_HDR_HDR_CRC_FIELD_OFF];
            rxdCrc16 = (rxdCrc16 << 8) + (byte)serRxBuff[UART_FRAME_HDR_HDR_CRC_FIELD_OFF + 1];
            Serial.println (calcCrc16,HEX);
            Serial.println (rxdCrc16,HEX);
            if (calcCrc16 != rxdCrc16) {
                for (idx = 0; idx < UART_FRAME_HDR_PYLD_CRC_FIELD_OFF - 1; idx++) {
                serRxBuff[idx] = serRxBuff[idx + 1];
                off = UART_FRAME_HDR_PYLD_CRC_FIELD_OFF - 1;
                readLen = 1;
                totReadLen = off;
                break;
            }
       }
            pyldLen = serRxBuff[UART_FRAME_HDR_PYLD_LEN_FIELD_OFF];
            //Serial.println(pyldLen);
            pyldLen = (pyldLen << 8) || serRxBuff[UART_FRAME_HDR_PYLD_LEN_FIELD_OFF + 1];
            currMsgType = serRxBuff[UART_FRAME_HDR_MSG_TYPE_FIELD_OFF];
            currMsgType = (currMsgType << 8) || serRxBuff[UART_FRAME_HDR_MSG_TYPE_FIELD_OFF + 1];
            Serial.println (currMsgType,HEX);
            Serial.println (pyldLen,HEX);
            readLen = pyldLen + UART_FRAME_HDR_PYLD_CRC_FIELD_LEN;
            break;
        }
        default:
        {
            Serial.println (currMsgType,HEX);
            Serial.println (expMsgType,HEX);
            processRcvdMsg(serRxBuff, UART_FRAME_HDR_LEN, currMsgType, pyldLen);
            if (currMsgType == expMsgType || expMsgType == SHIELD_MSG_TYPE_ANY || (currMsgType == SHIELD_MSG_TYPE_EXT_APP_DATA_IND) || hdrAckFlr) {
                done = 1;
                break;
            }
            readLen = UART_FRAME_HDR_PYLD_CRC_FIELD_OFF;
            //memset(serRxBuff, 0, sizeof(serRxBuff));
            totReadLen = 0;
            off = 0;
            break;
            }
        }
        while (done == 0x0);
    }**/
};

int External_address_info::buildSendHdr(int msgType, unsigned char * pyldBuff_p, int pyldLen) {
    unsigned char * buff_p = serTxBuff;
    unsigned short calcCrc16;
    static unsigned char seqNr = 0x0;
    int idx;
    int hdrAckFlr = 0;
    
    htons(buff_p, msgType);
    buff_p += UART_FRAME_HDR_MSG_TYPE_FIELD_LEN;

    *buff_p = 0x0;
    buff_p += UART_FRAME_HDR_FLAGS_FIELD_LEN;

    *buff_p = seqNr++;
    buff_p += UART_FRAME_HDR_SEQ_NR_FIELD_LEN;

    htons(buff_p, pyldLen);
    buff_p += UART_FRAME_HDR_PYLD_LEN_FIELD_LEN;
    
    calcCrc16 = crc16(serTxBuff, UART_FRAME_HDR_HDR_CRC_FIELD_OFF);
    htons(buff_p, calcCrc16); 
    buff_p += UART_FRAME_HDR_HDR_CRC_FIELD_LEN;
    if (pyldLen > 0) {
        calcCrc16 = crc16(pyldBuff_p, pyldLen);
        htons(buff_p, calcCrc16);
    } 
    else {
        htons(buff_p, 0x0); 
    }
    for(int idx=0; idx<10; idx++)
    {
      Serial.println(serTxBuff[idx],HEX);
      
    }
      Serial1.write(serTxBuff,10);
    
}


unsigned short External_address_info::crc16(unsigned char *buff_p, unsigned int len)
{
   unsigned long ckSum = 0;

   while (len > 1)
   {
      unsigned short tmp = *buff_p;
      tmp = (tmp << 8) | (*(buff_p + 1));
      ckSum = ckSum + tmp;
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



unsigned short External_address_info::htons(unsigned char * buff_p, unsigned short val) {
    
    buff_p[0] = (val >> 8) & 0xff;
    buff_p[1] = (val) & 0xff;
}

/**int External_address_info::processRcvdMsg(unsigned char * buff_p, int offset, int currMsgType, int pyldLen) {
    int idx;
    Serial.println(currMsgType,HEX);
    Serial.println(pyldLen,HEX);
    Serial.println(offset,HEX);

    for (idx = 0; idx < pyldLen; idx++) {
        if (idx % 8 == 0) {
            Serial.println(buff_p[offset + idx],HEX);
        }
    }

    switch (currMsgType) {
        case SHIELD_MSG_TYPE_NODE_EXT_ADDR_INFO:
        {
            if (pyldLen == LPWMN_MAC_EXT_ADDR_LEN) {
                Serial.println( * (buff_p + offset));
                Serial.println( * (buff_p + offset + 1));
                Serial.println( * (buff_p + offset + 2));
                Serial.println( * (buff_p + offset + 3));
                Serial.println( * (buff_p + offset + 4));
                Serial.println( * (buff_p + offset + 5));
                Serial.println( * (buff_p + offset + 6));
                Serial.println( * (buff_p + offset + 7));
            }
        }
            break;
            case SHIELD_MSG_TYPE_RF_BAUD_RATE_INFO:
            {
              if(pyldLen==1)
              {
                Serial.println(*(buff_p+offset),HEX);
              }
            }
            
            break;

            case SHIELD_MSG_TYPE_DATA_PUSH_INTERVAL_INFO:
            {
              if(pyldLen==4)
              {
                return buff_p+offset;
              }
            }
            break;

           /**case SHIELD_MSG_TYPE_NODE_INIT_DONE:
           {
            Serial print("Recieved Node Init Done Event\n");
           }
           break;**/

           /**case SHIELD_MSG_TYPE_NWK_SCAN_FLR_IND:
           {
            Serial.print("Recieved Nwk Scan Failure Event\n");
           }**/

           /**case SHIELD_MSG_TYPE_NWK_JOIN_IND:
           {
            if(pyldLen==LPWMN_MAC_SHORT_ADDR_LEN)
            {
              unsigned int shortAddr= UTIL_ntohs(buff_p+offset);
              Serial.println(shortAddr);
            }
           }
           break;**/
              
           /**case SHIELD_MSG_TYPE_EXT_APP_DATA_CNFRM:
           {
            if(pyldLen==SHIELD_DATA_CNFRM_STS_FIELD_LEN)
            {
              dataCnfrmSts=*(buff_p+offset);
              Serial.println(dataCnfrmSts);
            }
           }
           break;**/
           /**case UART_MSG_TYPE_ACK:
           {
            hdrFlags=buff_p[UART_FRAME_HDR_FLAGS_FIELD_OFF];
            Serial.println(hdrFlags);
           }
           break;**/

           
            
    /**   default:
        {
            Serial.println(currMsgType,HEX);
        }
    }**/

//}
/*
void serialEvent1()
{
  Serial.println("ok event");
  while(Serial1.available())
  {
   
    char inByte = Serial1.read();
    Serial.print("Read value is ");
    Serial.println((byte)inByte, HEX);
   }
}
*/

void External_address_info::print_usage()
{
  Serial.print("Usage wjn");
  Serial.print("On Cywgin, the serial port device is named /dev/ttySX if the COM port is COMY where X = Y - 1 \n");
}



void setup()
{
  //int argc;
  //const char*argv[1000];
  //int idx,msgCnt=0,msgLen=0,pktCnt=0;
  //int waitForNwkJoinFlag=0;
  //int waitForNodeInitFlag=0;
 Serial.begin(9600);
 Serial1.begin(9600);
 External_address_info obj1;
 External_address_info obj2;
 External_address_info obj3;
 External_address_info obj4;
 External_address_info obj5;
 External_address_info obj6;
 External_address_info obj7;
 External_address_info obj8;
 //External_address_info obj9;
 External_address_info obj10;
 External_address_info obj11;
 External_address_info obj12;
 
  /**if (argc<2)
  {
    obj9.print_usage();
    return 1;
  }

  if(argc==3)
  {
    if(strcmp(argv[2],"wnj")==0)
    {
      waitForNwkJoinFlag=1;
    }
    else
    {
      if(strcmp(argv[2],"wni"==0))
      {
        waitForNodeInitFlag=1;
        waitForNwkJoinFlag=1;
      }
      else
      {
        obj9.print_usage();
        return 1;
      }
      }**/
  
  

    /**if(waitForNodeInitFlag)
    {**/
      //Serial.print("Waiting for message from Wisense SHield");
      //obj8.readSerIntf(SHIELD_MSG_TYPE_NODE_INIT_DONE);
    //}**/
    Serial.print("short address external node build\n"); 
   obj1.sendNodeExtAddrInfoReq();
   Serial.println("Short external node address response\n");
   Serial1.readBytes(serRxBuff, 18);
  for(int i=0; i<18;i++)
  {
   Serial.println(serRxBuff[i],HEX);
  }

 //  obj2.readSerIntf(SHIELD_MSG_TYPE_GET_NODE_EXT_ADDR_INFO_REQ);
   delay(1000);
   Serial.print("Baud rate build\n");
   obj3.sendRFBaudRateInfoReq();
   Serial.print("Baud Rate response\n");
   Serial1.readBytes(serRxBuff, 11);
  for(int i=0; i<11;i++)
  {
   Serial.println(serRxBuff[i]);
  }
   //obj4.readSerIntf(SHIELD_MSG_TYPE_GET_RF_BAUD_RATE_INFO_REQ);
   delay(1000);
   Serial.print("dpi build\n");
   obj5.sendDPIInfoReq();
   Serial.print("DPI Interval response\n");
   Serial1.readBytes(serRxBuff, 14);
  for(int i=0; i<14;i++)
  {
    Serial.println(serRxBuff[i]);
  }
  //delay(1000);
  /**Serial.print("Node request building\n");
  obj7.sendNodeStartReq();
  Serial.print("Start the Node response\n");
  Serial1.readBytes(serRxBuff,10);
   for(int i=0;i<10;i++)
  {
    Serial.println(serRxBuff[i]);
  }**/
  delay(5000);
  Serial.print("Node Initialization build\n");
  obj10.sendNodeInitDone();
  Serial.print("Node Initialization done response\n");
  Serial1.readBytes(serRxBuff,10);
  for(int i=0;i<10;i++)
  {
    Serial.println(serRxBuff[i]);
  }
  delay(1000);
  Serial.print("Node request building\n");
  obj7.sendNodeStartReq();
  Serial.print("Start the Node response\n");
  Serial1.readBytes(serRxBuff,10);
   for(int i=0;i<10;i++)
  {
    Serial.println(serRxBuff[i]);
  }
  delay(2000);
  Serial.print("Ntwk Joining build\n");
  obj11.sendNwkjoininitialization();
  Serial.print("Network joining initialization response\n");
  Serial1.readBytes(serRxBuff,12);
  for(int i=0;i<12;i++)
  {
    Serial.println(serRxBuff[i]);
  }
  delay(1000);
  /**Serial.print("Nwk Scanning build\n");
  obj12.sendNwkScanInit();
  Serial.print("Networking Scanning Sucess response\n");
  Serial1.readBytes(serRxBuff,20);
  for(int i=0;i<20;i++)
  {
    Serial.println(serRxBuff[i]);
  }**/
   //delay(1000);
   //obj6.readSerIntf(SHIELD_MSG_TYPE_DATA_PUSH_INTERVAL_INFO);
   //delay(1000);
   /**obj7.sendNodeStartReq();
   for(int i=0;i<10;i++)
  {
    Serial.println(serRxBuff[i]);
  }**/
    delay(1000);
    Serial.println("In main loop");
    
  
  
 /** unsigned char *buff_p=serTxBuff;
char a='A';
while(1)**/
  /** {
    while(Serial.available())
  {
   
    char inByte = Serial1.read();
    Serial.print("Read value is ");
    Serial.println((byte)inByte, HEX);
   }
   }**/


 /**Serial.begin(9600);
 Serial1.begin(9600);
 External_address_info obj1;
 External_address_info obj2;
 External_address_info obj3;
 External_address_info obj4;
 External_address_info obj5;
 External_address_info obj6;
 External_address_info obj7;

 

 obj1.sendNodeExtAddrInfoReq();
 //while(Serial.readStringUntil(a));
 Serial.println("ok event");
  Serial1.readBytes(serRxBuff, 8);
  for(int i=0; i<8;i++)
  {
    Serial.println(serRxBuff[i]);
  }
  
   obj2.readSerIntf(SHIELD_MSG_TYPE_GET_NODE_EXT_ADDR_INFO_REQ);
   delay(1000);
   obj3.sendRFBaudRateInfoReq();
   Serial.print("Baud Rate");
   Serial1.readBytes(serRxBuff, 8);
  for(int i=0; i<8;i++)
  {
    Serial.println(serRxBuff[i]);
  }
   obj4.readSerIntf(SHIELD_MSG_TYPE_GET_RF_BAUD_RATE_INFO_REQ);
   delay(1000);
   obj5.sendDPIInfoReq();
   Serial.print("DPI Interval");
    Serial1.readBytes(serRxBuff, 8);
  for(int i=0; i<8;i++)
  {
    Serial.println(serRxBuff[i]);
  }
   
   obj6.readSerIntf(SHIELD_MSG_TYPE_DATA_PUSH_INTERVAL_INFO);
   delay(1000);
   obj7.sendNodeStartReq();

  Serial.println("In main loop");**/
 /**while(1)
   {
    while(Serial.available())
  {
   
    char inByte = Serial1.read();
    Serial.print("Read value is ");
    Serial.println((byte)inByte, HEX);
   }
   }**/

   

}



/**void External_address_info::print_usage()
{
  Serial.print("Usage wjn");
  Serial.print("On Cywgin, the serial port device is named /dev/ttySX if the COM port is COMY where X = Y - 1 \n");
}**/


void loop()
{
  
}

Wsnode
