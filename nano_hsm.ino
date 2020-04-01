
#include <Arduino.h>
#include <Wire.h>
#include <NanoCrypt.h>
#include <stdio.h>



int counter = 0;
#define PRINT_BUFFER_SIZE ((unsigned int)512)
char printbuffer[PRINT_BUFFER_SIZE];
#define CONVERSION_BUFFER_SIZE (unsigned int) 512
char conversionBuffer[CONVERSION_BUFFER_SIZE];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(2000000);
  Wire.begin();
  while (!Serial)  
    {
    }
 
  Serial.println ();
  Serial.println ("I2C Address Scanner. Scanning ...");
  byte count = 0;
  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
      {
      Serial.print ("Found i2c Device Address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);  
      } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");

  sprintf(printbuffer, "Getting Revision Info on Slave address: (0x%X):", Crypt.getSlaveAddress()); 
  Serial.println(printbuffer);
  uint8_t output_data[32];
  ATCA_STATUS status = Crypt.info(INFO_MODE_REVISION, 0, output_data);
  NanoCryptCommand* pCmd = Crypt.getCmd();
  Crypt.hexString(output_data, 4, conversionBuffer, CONVERSION_BUFFER_SIZE);
  sprintf(printbuffer, "Status: %d, Bytes recieved: %d, Revision: %s", status, pCmd->recieved, conversionBuffer);
  Serial.println(printbuffer);  

  sprintf(printbuffer, "Dumping config zone: (0x%X):", Crypt.getSlaveAddress()); 
  Serial.println(printbuffer);
  uint8_t zone = ATCA_ZONE_CONFIG;
  uint8_t slot = 0;
  
  uint8_t len = 32;
  for(uint8_t block = 0; block < 32; block++){
    status = Crypt.readZone(zone, slot, block, 0, output_data, len);
    Crypt.hexString(&(pCmd->txPacket.opcode), pCmd->txPacket.txsize, conversionBuffer, CONVERSION_BUFFER_SIZE);
    sprintf(printbuffer, "For block %d sent I2C data: [ %s ]", block, conversionBuffer); 
    Serial.println(printbuffer);
    if(status == ATCA_SUCCESS){
      Crypt.hexString(output_data, 32, conversionBuffer, CONVERSION_BUFFER_SIZE);
      sprintf(printbuffer, "Block %03d data: [ %s ]", block, conversionBuffer); 
      Serial.println(printbuffer);
    } else {
      sprintf(printbuffer, "Read Configuration Zone failed on block: %d with status 0x%X; Bytes recieved: %d", block, status, pCmd->recieved); 
      Serial.println(printbuffer);
    }
  }
  
}

void loop() {
  
}
