#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define SIGNAL_TIMEOUT 500  // This is signal timeout in milli seconds.

const uint64_t pipeIn = 0xF9E8F0F0E1LL;
RF24 radio(8, 9); 
unsigned long lastRecvTime = 0;

struct PacketData
{
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
  byte lPotValue;  
  byte rPotValue;    
  byte switch1Value;
  byte switch2Value;
  byte switch3Value;
  byte switch4Value;  
};
PacketData receiverData;

void setup()
{
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1,pipeIn);
  radio.startListening(); //start the radio receiver 
  Serial.begin(115200);
}

void loop()
{
  // Check if RF is connected and packet is available 
  if(radio.isChipConnected() && radio.available())
  {
    radio.read(&receiverData, sizeof(PacketData)); 
    lastRecvTime = millis(); 
     
    char inputValuesString[100];
    sprintf(inputValuesString, 
            "%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d",
             receiverData.lxAxisValue,
             receiverData.lyAxisValue,
             receiverData.rxAxisValue,
             receiverData.ryAxisValue,
             receiverData.lPotValue,
             receiverData.rPotValue,
             receiverData.switch1Value,
             receiverData.switch2Value,
             receiverData.switch3Value,
             receiverData.switch4Value);
    Serial.println(inputValuesString); 
  }
  else
  {  
    //Check Signal.
    unsigned long now = millis();
    if ( now - lastRecvTime > SIGNAL_TIMEOUT ) 
    {
      Serial.println("No Signal");      
    }
  }  
}
