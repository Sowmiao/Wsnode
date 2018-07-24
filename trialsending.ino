#include <Arduino.h>

using namespace std;

#define SER_BUFF_LEN 1024
unsigned char serTxBuff[SER_BUFF_LEN];//The total space allocated for serTxbuff is 1024
unsigned char serRxBuff[SER_BUFF_LEN];//The total space allocated for serRxBuff is 1024
//unsigned char Buff[8]={0xfc,0xc2,0x3d,0x00,0x00,0x10,0x6f,0x17};
/** Macro Definitions**/
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

#define UTIL_TLV_TYPE_FIELD_SZ   0x1
#define UTIL_TLV_LEN_FIELD_SZ    0x1

#define UTIL_TLV_HDR_SZ  (UTIL_TLV_TYPE_FIELD_SZ + UTIL_TLV_LEN_FIELD_SZ)
#define PLTFRM_DUMMY_DEV_ID        0x0

#define PLTFRM_AT24C01C_1_DEV_ID    0x1           
#define PLTFRM_AT24C01C_1_I2C_ADDR  0x0  
#define PLTFRM_AT24MAC602_DEV_CNT  1
#define PLTFRM_AT24MAC602_1_DEV_ID    0x2
#define PLTFRM_AT24MAC602_1_I2C_ADDR  0x0
#define PLTFRM_LM75B_1_DEV_ID       0x9            
#define PLTFRM_LM75B_1_I2C_ADDR     0x0
#define PLTFRM_LMP75B_DEV_CNT       0x1
#define PLTFRM_TMP102_1_DEV_ID      0xa            
#define PLTFRM_TMP102_1_I2C_ADDR    0x0            
#define PLTFRM_TMP102_DEV_CNT       0x1
#define PLTFRM_NXFT15XH103_1_DEV_ID 0xd       // Murata - NXFT15XH103 (analog)
#define PLTFRM_NTCALUG02A_1_DEV_ID  0xb       // Vishay - NTCALUG02A (analog)
#define PLTFRM_NTCALUG02A_2_DEV_ID  0xc       // Vishay - NTCALUG02A (analog)
#define PLTFRM_NXFT15XH103_2_DEV_ID 0xe   // Murata - NXFT15XH103 (analog)
#define PLTFRM_DNAX300R103L040_1_DEV_ID  0xf   // Deem
// Light sensor
#define PLTFRM_SFH_7773_1_DEV_ID    0x11           // I2C
#define PLTFRM_SFH_7773_1_I2C_ADDR  0x0
#define PLTFRM_SFH_7773_DEV_CNT     0x1            // Number of such devices in the system

// Light sensor
#define PLTFRM_TSL45315_1_DEV_ID    0x12           // I2C
#define PLTFRM_TSL45315_1_I2C_ADDR  0x0
#define PLTFRM_TSL45315_DEV_CNT     0x1            // Number of such devices in the system
   
// Gyro + Accelerometer + Temp
#define PLTFRM_MPU6050_1_DEV_ID     0x13           // I2C
#define PLTFRM_MPU6050_1_I2C_ADDR   0x0            // LSB is 0 (AD0 is 0)
#define PLTFRM_MPU6050_DEV_CNT      0x1            // Number of such devices in the system

// Accelerometer
#define PLTFRM_ADXL345_1_DEV_ID     0x14           // I2C and SPI
#define PLTFRM_ADXL345_DEV_CNT      0x1            // Number of such devices in the system

// SHT-10 (humidity + temperature)
#define PLTFRM_SHT10_1_TEMP_DEV_ID       0x15           // I2C
#define PLTFRM_SHT10_1_RH_DEV_ID         0x18           // I2C
#define PLTFRM_SHT10_DEV_CNT             0x1

// SHT-15 (humidity + temperature)
#define PLTFRM_SHT15_1_DEV_ID          0x22        // I2C
#define PLTFRM_SHT15_1_TEMP_DEV_ID     PLTFRM_SHT15_1_DEV_ID        // I2C
#define PLTFRM_SHT15_1_RH_DEV_ID       0x23        // I2C
#define PLTFRM_SHT15_DEV_CNT           0x1


#define LIS3MDLTR_I2C_ADDR_PREFIX   0x1c           // 0 0011100
// LIS3MDLTR (Magnetometer)
#define PLTFRM_LIS3MDLTR_1_DEV_ID   0x16           // I2C
#define PLTFRM_LIS3MDLTR_1_I2C_ADDR (LIS3MDLTR_I2C_ADDR_PREFIX | 0x0)  // 00111x0 , x is 0
#define PLTFRM_LIS3MDLTR_DEV_CNT    0x1


// RADIO
#define PLTFRM_CC2520_1_DEV_ID      0x19           // SPI
#define PLTFRM_CC1200_1_DEV_ID      0x1a           // SPI
#define PLTFRM_CC1101_1_DEV_ID      0x1b           // SPI

// Pressure sensor
#define PLTFRM_MPL115A1_1_DEV_ID    0x20           // SPI
#define PLTFRM_MS5637_1_DEV_ID      0x21

#define PLTFRM_P43_US_1_DEV_ID  0x24


#define PLTFRM_MAX_SONAR_DEV_CNT    3              // MAXBOTIX ultrasonic
#define PLTFRM_MAX_SONAR_1_DEV_ID   0x26
#define PLTFRM_MAX_SONAR_2_DEV_ID   0x27
#define PLTFRM_MAX_SONAR_3_DEV_ID   0x28

#define PLTFRM_AS339_1_DIVER_PRESSURE_DEV_ID  0x2c
#define PLTFRM_AS339_1_DIVER_TEMPERATURE_DEV_ID  0x2d
#define PLTFRM_AS339_1_DIVER_MOD_PRESSURE_DEV_ID  0x2e
#define PLTFRM_AS339_1_DIVER_MOD_TEMPERATURE_DEV_ID  0x2f

#define PLTFRM_EKMC160111X_1_DEV_ID   0x30         // GPIO
#define PLTFRM_EKMC160111X_DEV_CNT    0x1          // Number of such devices in the system

#define PLTFRM_PULSE_CNTR_1_DEV_ID  0x34
#define PLTFRM_PULSE_CNTR_2_DEV_ID  0x35

#define PLTFRM_GEN_VOLT_MON_DEV_ID   0x38 
#define PLTFRM_GEN_CURRENT_MON_DEV_ID 0x39 

#define PLTFRM_EXT_VOLTAGE_MON_DEV_ID   0x3a

// LEDs
#define PLTFRM_LED_1_DEV_ID           0x40         // GPIO
#define PLTFRM_LED_2_DEV_ID           0x41         // GPIO

#define PLTFRM_BAR_CODE_SCANNER_1_DEV_ID   0x48    // UART

#define PLTFRM_AUTO_ASSY_TMON_1_DEV_ID  0x4a

// Switches (On/Off)
#define PLTFRM_REED_SWITCH_1_DEV_ID   0x50         // GPIO
#define PLTFRM_REED_SWITCH_DEV_CNT    0x1

#define PLTFRM_SPST_SWITCH_1_DEV_ID   0x51         // GPIO
#define PLTFRM_SPST_SWITCH_DEV_CNT    0x1

#define PLTFRM_DUAL_FS_LVL_MON_1_DEV_ID  0x52  

#define PLTFRM_ACS712_1_CURRENT_DEV_ID   0x54      // Analog
#define PLTFRM_ACS712_2_CURRENT_DEV_ID   0x55      // Analog
#define PLTFRM_ACS712_3_CURRENT_DEV_ID   0x56      // Analog
#define PLTFRM_ACS712_DEV_CNT  3


// Analog Pressure Sensor
#define PLTFRM_MP3V5050GP_1_DEV_ID    0x60         // Analog input
#define PLTFRM_MP3V5050GP_DEV_CNT     0x1

#define PLTFRM_HE055T01_1_DEV_ID      0x61         // Analog input
#define PLTFRM_HE055T01_DEV_CNT       0x1

#define PLTFRM_MP3V5004GP_1_DEV_ID    0x62         // Analog input
#define PLTFRM_MP3V5004GP_DEV_CNT     0x1

#define PLTFRM_MPXV5010G_1_DEV_ID     0x63         // Analog input
#define PLTFRM_MPXV5010G_DEV_CNT      0x1

#define PLTFRM_MP3V5010_1_DEV_ID      0x64
#define PLTFRM_MP3V5010_DEV_CNT       1

#define PLTFRM_LLS_1_DEV_ID           0x65

#define PLTFRM_BATT_1_DEV_ID          0x68         // Analog input
#define PLTFRM_BATT_DEV_CNT           0x1

#define PLTFRM_4_20_CURRENT_SENSOR_ID    0x69       // Analog input
#define PLTFRM_4_20_CURRENT_SENSOR_CNT   0x1

#define PLTFRM_SETRA_3100_1_DEV_ID    0x6a
#define PLTFRM_SETRA_3100_DEV_CNT     1

#define PLTFRM_DS18B20_1_DEV_ID     0x6c

// AD7797 (Bridge sensor)
#define PLTFRM_AD7797_1_DEV_ID        0x70
#define PLTFRM_AD7797_DEV_CNT         0x1          // SPI bus

#define PLTFRM_ON_CHIP_VCC_SENSOR_DEV_ID    0x78      // ADC_10 channel
#define PLTFRM_ON_CHIP_VCC_SENSOR_DEV_CNT   0x1

#define PLTFRM_ON_CHIP_TEMP_SENSOR_DEV_ID    0x79      // ADC_10 channel
#define PLTFRM_ON_CHIP_TEMP_SENSOR_DEV_CNT   0x1


#define PLTFRM_SYNC_RT_1_DEV_ID  0x80
#define PLTFRM_SYNC_RT_DEV_CNT   0x1

#define PLTFRM_MAG3110_1_DEV_ID  0x88

#define PLTFRM_INA219_1_BV_DEV_ID     0x58         // I2C   INA219 bus voltage
#define PLTFRM_INA219_1_SV_DEV_ID     0x59         // I2C   INA219 shunt voltage
#define PLTFRM_INA219_DEV_CNT         0x1

#define PLTFRM_INA219_3_BV_DEV_ID     0x5c         // I2C   INA219 bus voltage
#define PLTFRM_INA219_3_SV_DEV_ID     0x5d         // I2C   INA219 shunt voltage


#define PLTFRM_GPIO_LOGIC_SZ  0x1
#define PLTFRM_LOGIC_1_DEV_ID  0x90
// #define PLTFRM_LOGIC_2_DEV_ID  0x91

#define PLTFRM_GPIO_REMOTE_CTRL_DEV_ID  0x90

#define PLTFRM_CHIRP_PWLA_1_DEV_ID  0x91
#define PLTFRM_CHIRP_PWLA_DEV_CNT 1

#define PLTFRM_FC_28_1_DEV_ID  0x92
#define PLTFRM_FC_28_DEV_CNT  1

#define PLTFRM_WSMS100_1_DEV_ID  0x93

#define PLTFRM_CC2D33S_1_RH_DEV_ID      0xb0  // I2C
#define PLTFRM_CC2D33S_1_TEMP_DEV_ID    0xb1  // I2C

#define PLTFRM_MPL3115A2_1_DEV_ID  0xb8  // I2C
#define PLTFRM_MPL3115A2_DEV_CNT   0x1

#define PLTFRM_MPL115A2_1_DEV_ID  0xb9  // I2C
#define PLTFRM_MPL115A2_DEV_CNT  0x1

#define PLTFRM_BMP180_1_DEV_ID  0xba  // I2C
#define PLTFRM_BMP180_DEV_CNT  0x1

#define PLTFRM_HPM_1_PM2PT5_DEV_ID  0xbe
#define PLTFRM_HPM_1_PM10_DEV_ID  0xbf

#define PLTFRM_MMA7660FC_1_DEV_ID  0xc0  // I2C
#define PLTFRM_MMA7660FC_DEV_CNT   0x1

#define PLTFRM_MDS_1_DEV_ID  0xca

#define PLTFRM_VIBRATION_SNSR_1_DEV_ID  0xcc

#define PLTFRM_I2C_SW_BUS_1_DEV_ID  0xe0
#define PLTFRM_I2C_SW_BUS_2_DEV_ID  0xe1

#define PLFRM_SPI_HW_BUS_1_DEV_ID   0xe8

#define PLTFRM_WS_VEH_DET_1_DEV_ID  0xea

#define PLTFRM_DEV_TYPE_SOLAR_PWR_SRC_VSENSE  0xf0
#define PLTFRM_DEV_TYPE_SOLAR_PWR_SRC_ISENSE  0xf1

#define PLTFRM_SOL_PSU_1_DEV_ID  0xf2
#define PLTFRM_SOL_PSU_1_VSOL_DEV_ID  0xf2
#define PLTFRM_SOL_PSU_1_VBATT_DEV_ID  0xf3
#define PLTFRM_SOL_PSU_1_VSYS_DEV_ID  0xf4
#define PLTFRM_SOL_PSU_1_ISOL_DEV_ID  0xf5
#define PLTFRM_SOL_PSU_1_IBATT_DEV_ID  0xf6

#define PLTFRM_WPDS_DEV_ID  0xfb
#define PLTFRM_32KHZ_CRYSTAL_DEV_ID  0xfe

#define PLTFRM_GENERIC_DEV_ID      0xff

// --------------------------------------------------------- 
   
   
// ------------- Buses (I2C, SPI, 1-WIRE etc) --------------   
// I2C (software)
#define PLTFRM_I2C_SW_BUS_1_ID      0x80

   
// I2C (hardware) 
#define PLFRM_I2C_HW_BUS_1_ID      0x88

#define PLTFRM_INV_I2C_BUS_ID      0xff
#define PLTFRM_INV_I2C_ADDR        0xff
 
// SPI (hardware)
#define PLFRM_SPI_HW_BUS_1_ID      0x90

// 1 WIRE (software)
#define PLTFRM_1WIRE_SW_BUS_1_ID   0x98
   
// UART
#define PLTFRM_UART_HW_1_DEV_ID    0xa0
// ---------------------------------------------------------   


#define NTC_THERM_103AT_4_R25_VAL  10000
#define NTC_THERM_103AT_4_B_25_85_VAL   3435

#define NTC_THERM_NTCALUG02A_R25_VAL  10000
#define NTC_THERM_NTCALUG02A_B_25_85_VAL   3984


#define NTC_THERM_NXFT15XH103_R25_VAL  10000
#define NTC_THERM_NXFT15XH103_B_25_85_VAL   3380

#define NTC_THERM_DNAX300R103L040_4_R25_VAL  10000
#define NTC_THERM_DNAX300R103L040_4_B_25_85_VAL  3950

#define PLTFRM_TEPT5700_1_DEV_ID   0x10   // Vishay light sensor

#define DIS_TLV_TYPE_FIELD_LEN    1
#define DIS_TLV_LENGTH_FIELD_LEN  1

#define DIS_LPWMN_MAC_SHORT_ADDR_LEN  2

#define DIS_TLV_HDR_SZ  (DIS_TLV_TYPE_FIELD_LEN  + DIS_TLV_LENGTH_FIELD_LEN)

#define DIS_SENSOR_ID_ALL  0xff

#define DIS_SENSOR_ID_FIELD_SZ           1
#define DIS_DATA_SCALE_FACTOR_FIELD_SZ   1
#define DIS_SENSOR_TYPE_FIELD_SZ         1
#define DIS_DATA_UNIT_FIELD_SZ           1
#define DIS_SENSOR_CNT_FIELD_SZ          1
#define DIS_SENSOR_OPN_MODE_BIT_MSK_SZ   1

#define DIS_NODE_PWR_SRC_FIELD_SZ        1
#define DIS_BATT_CAP_MAH_FIELD_SZ        2

#define DIS_STS_FIELD_SZ                 1

#define DIS_TIME_IN_NANOSECS_FIELD_SZ    2    // 0 - 999
#define DIS_TIME_IN_MICROSECS_FIELD_SZ   2    // 0 - 999
#define DIS_TIME_IN_MILLISECS_FIELD_SZ   2    // 0 - 999
#define DIS_TIME_IN_SECS_FIELD_SZ        1    // 0 - 59
#define DIS_TIME_IN_MINUTES_FIELD_SZ     1    // 0 - 59
#define DIS_TIME_IN_HOURS_FIELD_SZ       1    // 0 - 23
#define DIS_TIME_IN_DAYS_FIELD_SZ        1    // 0 - 255

#define DIS_SENSOR_COUNT_FIELD_SZ        1    // 0 - 255

#define DIS_SENSOR_ID_TLV_SZ  (DIS_TLV_HDR_SZ + DIS_SENSOR_ID_FIELD_SZ)
#define DIS_DATA_SCALE_FACTOR_TLV_SZ (DIS_TLV_HDR_SZ + DIS_DATA_SCALE_FACTOR_FIELD_SZ)
#define DIS_SENSOR_TYPE_TLV_SZ (DIS_TLV_HDR_SZ + DIS_SENSOR_TYPE_FIELD_SZ)
#define DIS_DATA_UNIT_TLV_SZ (DIS_TLV_HDR_SZ + DIS_DATA_UNIT_FIELD_SZ)
#define DIS_SENSOR_OPN_MODE_TLV_SZ (DIS_TLV_HDR_SZ + DIS_SENSOR_OPN_MODE_BIT_MSK_SZ) 
#define DIS_TIME_IN_NANOSECS_TLV_SZ      (DIS_TLV_HDR_SZ + 2)    // 0 - 999
#define DIS_TIME_IN_MICROSECS_TLV_SZ     (DIS_TLV_HDR_SZ + 2)    // 0 - 999
#define DIS_TIME_IN_MILLISECS_TLV_SZ     (DIS_TLV_HDR_SZ + 2)    // 0 - 999
#define DIS_TIME_IN_SECS_TLV_SZ          (DIS_TLV_HDR_SZ + 1)    // 0 - 59
#define DIS_TIME_IN_MINUTES_TLV_SZ       (DIS_TLV_HDR_SZ + 1)    // 0 - 59
#define DIS_TIME_IN_HOURS_TLV_SZ         (DIS_TLV_HDR_SZ + 1)    // 0 - 23
#define DIS_TIME_IN_DAYS_TLV_SZ          (DIS_TLV_HDR_SZ + 1)    // 0 - 255

#define DIS_STS_TLV_SZ  (DIS_TLV_HDR_SZ + DIS_STS_FIELD_SZ)
#define DIS_SENSOR_CNT_TLV_SZ  (DIS_TLV_HDR_SZ + DIS_SENSOR_CNT_FIELD_SZ)
#define DIS_PWR_SRC_TLV_SZ  (DIS_TLV_HDR_SZ + DIS_NODE_PWR_SRC_FIELD_SZ)
#define DIS_BATT_CAP_MAH_TLV_SZ (DIS_TLV_HDR_SZ + DIS_BATT_CAP_MAH_FIELD_SZ)

#define DIS_SENSOR_COUNT_TLV_SZ          (DIS_TLV_HDR_SZ + DIS_SENSOR_COUNT_FIELD_SZ)

#define DIS_SHORT_ADDR_TLV_SZ   (DIS_TLV_HDR_SZ + DIS_LPWMN_MAC_SHORT_ADDR_LEN)

#define DIS_DIGITAL_IO_PORT_FIELD_SZ  1
#define DIS_DIGITAL_IO_PIN_FIELD_SZ  1
#define DIS_DIGITAL_IO_VAL_FIELD_SZ  1

#define DIS_DIGITAL_IO_PORT_TLV_SZ   (DIS_TLV_HDR_SZ + DIS_DIGITAL_IO_PORT_FIELD_SZ)
#define DIS_DIGITAL_IO_PIN_TLV_SZ    (DIS_TLV_HDR_SZ + DIS_DIGITAL_IO_PIN_FIELD_SZ)
#define DIS_DIGITAL_IO_VAL_TLV_SZ    (DIS_TLV_HDR_SZ + DIS_DIGITAL_IO_VAL_FIELD_SZ)

#define DIS_GPIO_PORT_ID_FIELD_SZ  1
#define DIS_GPIO_PIN_ID_FIELD_SZ  1
#define DIS_GPIO_VAL_FIELD_SZ  1
#define DIS_GPIO_PORT_OP_STATE_FIELD_SZ  1

#define DIS_GPIO_PORT_ID_TLV_SZ   (DIS_TLV_HDR_SZ + DIS_GPIO_PORT_ID_FIELD_SZ)
#define DIS_GPIO_PIN_ID_TLV_SZ    (DIS_TLV_HDR_SZ + DIS_GPIO_PIN_ID_FIELD_SZ)
#define DIS_GPIO_VAL_TLV_SZ    (DIS_TLV_HDR_SZ + DIS_GPIO_VAL_FIELD_SZ)

#define DIS_GPIO_PORT_OP_STATE_TLV_SZ  (DIS_TLV_HDR_SZ + DIS_GPIO_PORT_OP_STATE_FIELD_SZ)

#define DIS_GPIO_CTRL_TLV_SZ (DIS_TLV_HDR_SZ \
                              + DIS_GPIO_PORT_ID_TLV_SZ \
                              + DIS_GPIO_PIN_ID_TLV_SZ \
                              + DIS_GPIO_VAL_TLV_SZ)

#define DIS_ATTR_ID_FIELD_SZ   2  

#define DIS_ATTR_ID_TLV_SZ   (DIS_TLV_HDR_SZ + DIS_ATTR_ID_FIELD_SZ) 

#define DIS_LIST_ATTR_ID_FIELD_SZ   2
#define DIS_LIST_ATTR_ID_TLV_SZ   (DIS_TLV_HDR_SZ + DIS_LIST_ATTR_ID_FIELD_SZ)

#define DIS_LIST_ATTR_IDX_FIELD_SZ   2
#define DIS_LIST_ATTR_IDX_TLV_SZ   (DIS_TLV_HDR_SZ + DIS_LIST_ATTR_IDX_FIELD_SZ)


#define DIS_MSG_TYPE_SZ   0x1

#define DIS_LQI_TLV_FIELD_SZ  1
#define DIS_RSSI_TLV_FIELD_SZ  1
#define DIS_RCVD_PKT_CNT_TLV_FIELD_SZ  2
#define DIS_SECONDS_SINCE_PKT_RX_TLV_FIELD_SZ  2

#define DIS_LQI_TLV_SZ  (DIS_TLV_HDR_SZ + DIS_LQI_TLV_FIELD_SZ)
#define DIS_RSSI_TLV_SZ  (DIS_TLV_HDR_SZ + DIS_RSSI_TLV_FIELD_SZ)
#define DIS_RCVD_PKT_CNT_TLV_SZ  (DIS_TLV_HDR_SZ + DIS_RCVD_PKT_CNT_TLV_FIELD_SZ)
#define DIS_SECONDS_SINCE_PKT_RX_TLV_SZ  (DIS_TLV_HDR_SZ + DIS_SECONDS_SINCE_PKT_RX_TLV_FIELD_SZ)


#define DIS_FU_PAGE_SZ_FIELD_LEN  1

#define DIS_TIME_STAMP_TLV_VAL_LEN   4
#define DIS_TIME_STAMP_TLV_SZ  (DIS_TLV_HDR_SZ + DIS_TIME_STAMP_TLV_VAL_LEN)

#define DIS_FU_PAGE_SZ_TLV_SZ  (DIS_TLV_HDR_SZ + DIS_FU_PAGE_SZ_FIELD_LEN)
#define DIS_UC_FU_STATE_TLV_SZ  (DIS_TLV_HDR_SZ + 1)
#define DIS_UC_FU_SEG_IDX_TLV_SZ  (DIS_TLV_HDR_SZ + 1)
#define DIS_UC_FU_PAGE_IDX_TLV_SZ  (DIS_TLV_HDR_SZ + 2)
#define DIS_UC_FU_RET_CODE_TLV_SZ  (DIS_TLV_HDR_SZ + 1)
#define DIS_FW_SEG_START_TLV_SZ  (DIS_TLV_HDR_SZ + 2)
#define DIS_FW_SEG_LEN_TLV_SZ  (DIS_TLV_HDR_SZ + 2)
#define DIS_FW_SEG_CRC_TLV_SZ  (DIS_TLV_HDR_SZ + 2)
#define DIS_UC_FU_PAGE_CRC_TLV_SZ (DIS_TLV_HDR_SZ + 2)

#define DIS_HOP_COUNT_TLV_SZ  (DIS_TLV_HDR_SZ + 1)

#define DIS_REBOOT_DELAY_TLV_SZ  (DIS_TLV_HDR_SZ + 2)
#define DIS_TLV_TYPE_SENSOR_ID 20

#define DIS_TLV_TYPE_VALUE 2

#define DIS_TLV_TYPE_SENSOR_OUTPUT 17

#define DIS_TLV_TYPE_SENSOR_OUTPUT_LIST 16


class External_address_info 
{
  public:
  //int processRcvdMsg(unsigned char *,int,int,int);
  //unsigned short dummyTempSnsrVal = 200; 
  
  unsigned char UTIL_buildTlvHdr(unsigned char *, unsigned char , unsigned char );//Function to determine the message type being sent as well as the payload length of the data being transmitted
  unsigned char UTIL_htons(unsigned char *, unsigned short );//Function to populate a buffer with the data that is to be sent.
  //unsigned char 
  int buildSendHdr(int , unsigned char * , int );//Building the data header(10 bytes)
  //void print_usage();
  unsigned short crc16(unsigned char * , unsigned int );//Function to check for the existence of errors in the data
  //int processRcvdMsg(unsigned char *,int,int,int);
  unsigned short htons(unsigned char * , unsigned short );//Function to cslculate the first two significant values of the buffer
  void sendNodeExtAddrInfoReq(void) 
  {
      buildSendHdr(SHIELD_MSG_TYPE_GET_NODE_EXT_ADDR_INFO_REQ, NULL, 0);//Building of header while trying to send an extended short address data
  }
   void sendNodeStartReq(void)
 {
  buildSendHdr(SHIELD_MSG_TYPE_START_NODE_REQ, NULL, 0);//Building header for node request
 }

 void sendNodeInitDone(void)
 {
  buildSendHdr(SHIELD_MSG_TYPE_NODE_INIT_DONE,NULL,0);//Node Initialization
 }

 void sendNwkjoininitialization(void)
 {
  buildSendHdr(SHIELD_MSG_TYPE_NWK_JOIN_IND,NULL,0);//Flag building header once, node joins the network
 }

 int sendDataTxReq(void)
 {
  static int dataTxCnt=0;
  unsigned short dummyTempSnsrVal = 200; 
  int pyldLen;
  unsigned char *buff_p;
  unsigned char *tlvList_p;
  unsigned char *pyld_p;

  pyldLen=UTIL_TLV_HDR_SZ+UTIL_TLV_HDR_SZ+DIS_SENSOR_ID_TLV_SZ+UTIL_TLV_HDR_SZ+2;//building the desired payload length of the data that is to be sent

  pyld_p=(unsigned char*)malloc(pyldLen);//dynamically allocating memory to store all attributes associated with the payload 

  if(pyld_p==NULL)
  {
    Serial.print("Payload length is\n");
    Serial.println(pyldLen);                     /**Extrcating the payload length from the data to be transmitted **/
    return -1;
  }
  buff_p=pyld_p;//Assigning the contents of pyld_p onto the buffer
  buff_p+=UTIL_TLV_HDR_SZ;//then subsequent reading of all the bytes going to be stores in buffer with an offset of 2

  {
    unsigned char *tlvHdr_p=buff_p;//Introducing a variable that can evaluate the type,length and value of the data which is assigned with the contents in the buffer 
    buff_p+=UTIL_TLV_HDR_SZ;//then subsequent reading of all the bytes going to be stores in buffer with an offset of 2

    buff_p=UTIL_buildTlvHdr(buff_p,DIS_TLV_TYPE_SENSOR_ID,DIS_SENSOR_ID_FIELD_SZ);
   *(buff_p++)=PLTFRM_LM75B_1_DEV_ID;

   buff_p=UTIL_buildTlvHdr(buff_p,DIS_TLV_TYPE_VALUE,2);  /** Building of header and payload of the data to be transmitted for initializing the function of data requesting by the node**/
   buff_p=UTIL_htons(buff_p, dummyTempSnsrVal);
   dummyTempSnsrVal ++;

   if(dummyTempSnsrVal > 500)
      dummyTempSnsrVal=200;

   UTIL_buildTlvHdr(tlvHdr_p,DIS_TLV_TYPE_SENSOR_OUTPUT,buff_p-tlvHdr_p-UTIL_TLV_HDR_SZ);
   
  }
  UTIL_buildTlvHdr(tlvList_p,DIS_TLV_TYPE_SENSOR_OUTPUT_LIST,buff_p-tlvList_p-UTIL_TLV_HDR_SZ);
  delay(2000);
  dataTxCnt++;
  Serial.print("Data Request being sent to Wisense Shield\n");
  Serial.print("Data Transmit counter number\n");
  Serial.println(dataTxCnt);
  Serial.print("Payload len\n");
  Serial.println(pyldLen);
  Serial.print("Building The sensor header\n");
  buildSendHdr(SHIELD_MSG_TYPE_EXT_APP_DATA_REQ,pyld_p,pyldLen);


  free(pyld_p);                                      /**Heap eased out with data**/
  Serial.print("Ok done\n");
  
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
 
 

}; /**Class definition**/






int External_address_info::buildSendHdr(int msgType, unsigned char * pyldBuff_p, int pyldLen) 
{
    unsigned char * buff_p = serTxBuff;
    unsigned short calcCrc16;
    static unsigned char seqNr = 0x0;
    int idx;
    int hdrAckFlr = 0;
    
    htons(buff_p, msgType);
    buff_p += UART_FRAME_HDR_MSG_TYPE_FIELD_LEN;

    *buff_p = 0x0;
    buff_p += UART_FRAME_HDR_FLAGS_FIELD_LEN;

    *buff_p = seqNr++;                                                  /**General Format for building sensor data header**/ 
    buff_p += UART_FRAME_HDR_SEQ_NR_FIELD_LEN;

    htons(buff_p, pyldLen);
    buff_p += UART_FRAME_HDR_PYLD_LEN_FIELD_LEN;
    
    calcCrc16 = crc16(serTxBuff, UART_FRAME_HDR_HDR_CRC_FIELD_OFF);
    htons(buff_p, calcCrc16); 
    buff_p += UART_FRAME_HDR_HDR_CRC_FIELD_LEN;                        /**CRC Calculation**/
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
      Serial1.write(serTxBuff,10);                                  /**Begining of transmission of data**/
    
}
unsigned char External_address_info:: UTIL_buildTlvHdr(unsigned char *tlv_p, unsigned char type, unsigned char pyldLen)
{
   *(tlv_p ++) = type;
   *(tlv_p ++) = pyldLen;
   return tlv_p;
}

unsigned char External_address_info::UTIL_htons(unsigned char *buff_p, unsigned short val)
{
   *(buff_p + 1) = val;
   val >>= 8;
   *(buff_p) = val;
   return (buff_p + 2);
}



unsigned short External_address_info::crc16(unsigned char *buff_p, unsigned int len)//CRC Calculation function
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



unsigned short External_address_info::htons(unsigned char * buff_p, unsigned short val) 
{
    
    buff_p[0] = (val >> 8) & 0xff;
    buff_p[1] = (val) & 0xff;
}

/**int External_address_info::processRcvdMsg(unsigned char * buff_p, int offset, int currMsgType, int pyldLen)
{
  unsigned char dataCnfrmSts = 0;
  int idx;
  delay(1000);

  Serial.println(currMsgType);
  Serial.println(pyldLen);
  Serial.println(offset);

  for(idx=0;idx<pyldLen;idx++)
  {
    if(idx%8==0)
    {
      Serial.print("Function\n");
    }
    Serial.println(buff_p[offset+idx]);
  }
   switch(currMsgType)
   {
    /**case SHIELD_MSG_TYPE_DATA_PUSH_INTERVAL_INFO:
    {
      if(pyldLen==4)
      {
        Serial.print("DPI Interval\n");
        Serial.println((buff_p+offset));
      }
    }
    break;**/

     /**case SHIELD_MSG_TYPE_RF_BAUD_RATE_INFO:
     {
      if(pyldLen==1)
      {
        Serial.print("Node baud rate encoding\n");
        Serial.println(*(buff_p+offset));
      }
     }
     break;**/

    /** case SHIELD_MSG_TYPE_NODE_EXT_ADDR_INFO:
     {
      if(pyldLen==LPWMN_MAC_EXT_ADDR_LEN)
      {
        Serial.print("Node Extended address\n");
        Serial.println(*(buff_p+offset));
        Serial.println(*(buff_p+offset+1));
        Serial.println(*(buff_p+offset+2));
        Serial.println(*(buff_p+offset+3));
        Serial.println(*(buff_p+offset+4));
        Serial.println(*(buff_p+offset+5));
        Serial.println(*(buff_p+offset+6));
        Serial.println(*(buff_p+offset+7));
      }
     }
     break;
     case SHIELD_MSG_TYPE_NODE_INIT_DONE:
     {
      Serial.print("Node init done\n");
     }
     break;
     case SHIELD_MSG_TYPE_EXT_APP_DATA_CNFRM:
     {
      if(pyldLen==SHIELD_DATA_CNFRM_STS_FIELD_LEN)
      {
        Serial.print("Recieved data cnfrm\n");
        Serial.println(dataCnfrmSts);
      }
     }
     break;

    
   }
}**/


  
 

void setup() 
{
  
  unsigned char Buff1[8]={0xfc,0xc2,0x3d,0,0,0x10,0xbc,0x27};         /**Defining desired payloads to be sent**/ 
  unsigned char Buff[8]={0xfc,0xc2,0x3d,0x00,0x00,0x10,0x6f,0x17};
  //Buff[8] = 0x43;
  Serial.begin(9600);
  Serial1.begin(9600);

  External_address_info obj1;
  External_address_info obj2;
  External_address_info obj3;
  External_address_info obj4;
  External_address_info obj5;

  /**Serial.print("Sending short address header of 10 bytes\n");
  obj1.sendNodeExtAddrInfoReq();**/
  Serial.print("Waiting for ACK from node\n");
  delay(2000);
  Serial.print("Rcvd ACK\n");
  delay(2000);
  Serial.print("Node Initialization build\n");
  obj2.sendNodeInitDone();
  Serial.print("Node Initialization done response\n");
  Serial1.readBytes(serRxBuff,10);
  for(int i=0;i<10;i++)
  {
    Serial.println(serRxBuff[i]);
  }
  delay(1000);
  Serial.print("Node request building\n");
  obj3.sendNodeStartReq();
  Serial.print("Start the Node response\n");
  Serial1.readBytes(serRxBuff,10);
   for(int i=0;i<10;i++)
  {
    Serial.println(serRxBuff[i]);
  }
  delay(2000);
  //Serial.print("Short address sending\n");
  //memset(serTxBuff,0,sizeof(serTxBuff));
  Serial.print("Sending short address header of 10 bytes\n");
  obj1.sendNodeExtAddrInfoReq();
  //delay(1000);
  //Serial.print("Short address sending 8  bytes of payload\n");
  //Serial.println(Buff[8]);
  //strcpy(serTxBuff,Buff[8]);/**Copyiny the 8 bytes header created onto the transmit buffer**/

  //Serial.println(serTxBuff);
   /**for(int j=0;j<8;j++)
  {
    Serial.println(serTxBuff[j],HEX);
  }
 // strcpy(serRxBuff,serTxBuff[j]);/**Writing it onto the recieve buffer**/
  //}**/
  
  //memset(serTxBuff,0,sizeof(serTxBuff));
  //strncat(serTxBuff,Buff,8);
  //strcpy(serRxBuff,Buff,8);  
  /**for(int j=0;j<8;j++)
 { 
    Serial.println(serTxBuff[j],HEX );
 }
  strncat(serRxBuff,serTxBuff,8);
  Serial.print("Copied from txbuffer to rxbuffer\n");
  for(int i=0;i<8;i++)
  {
  Serial.println(serRxBuff[i],HEX);
  }**/
  //Serial.print("Ok event");
  /**for(int i=0;i<8;i++)
  {
    //Serial.print("Ok event");
    Serial1.println(serRxBuff[i],HEX);
  }**/

  /**for(int i=0;i<8;i++)
  {
    Serial.println(serTxBuff[i],HEX);
  }**/

  /**delay(2000);
  Serial.print("response obtained\n");

  Serial1.readBytes(serRxBuff,18);/**
  Reading the response now from the buffer**/ 

 /** for(int i=0;i<18;i++)
  {
    Serial.println(serRxBuff[i],HEX);
  }**/
  //delay(5000);
  obj4.sendDataTxReq();
  delay(1000);
  Serial.print("Short address sending 8  bytes of payload\n");
  
  strncat(serTxBuff,Buff,8);
  Serial.print("txbuffer contents\n");
  for(int i=0;i<8;i++)
  {
    Serial.println(serTxBuff[i],HEX);
  }
  
  Serial.print("Copied from txbuffer to rxbuffer\n");
  for(int i=0;i<8;i++)
  {
  Serial.println(serRxBuff[i],HEX);
  }
  delay(2000);
  Serial.print("response obtained\n");

  Serial1.readBytes(serRxBuff,18);/**Reading the response now from the buffer**/

  for(int i=0;i<18;i++)
  {
    Serial.println(serRxBuff[i],HEX);
  }
  
  Serial.println("the response due to datatxreq\n");
  Serial1.readBytes(serRxBuff,11);
  for(int i=0; i<11;i++)
  {
    //Serial.println(serRxBuff[i],HEX);
  //for(int j=0;j<11;j++)
  
  if(serRxBuff[i]== Buff1    )
  {
    Serial.print("Data is similar and thus a sucess\n");

    Serial.println(serRxBuff[i],HEX);
    Serial.println(Buff1[i],HEX);
    
  }
  else
  {
    Serial.print("Unsucessful result\n");
  }
  }
}
  
  //obj5.readSerIntf(SHIELD_MSG_TYPE_EXT_APP_DATA_CNFRM);
  

  


void loop() 
{
  // put your main code here, to run repeatedly:

}


/**NOTE: The data has to be checked with the gateway through certain linux commands on the cygwin terminal**/
