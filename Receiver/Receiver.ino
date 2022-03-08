#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define SIGNAL_TIMEOUT 500  // This is signal timeout in milli seconds. We will reset the data if no signal

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

Servo servo1;     //Pin D2
Servo servo2;     //Pin D3
Servo servo3;     //Pin D4
Servo servo4;     //Pin D5
Servo servo5;     //Pin D6
Servo servo6;     //Pin D7
int   led1 = A0;
int   led2 = A1;
int   led3 = A2;
int   led4 = A3;

//Assign default input received values
void setInputDefaultValues()
{
  // The middle position for joystick. (254/2=127)
  receiverData.lxAxisValue = 127;
  receiverData.lyAxisValue = 127;
  receiverData.rxAxisValue = 127;
  receiverData.ryAxisValue = 127;
  receiverData.lPotValue = 0;  
  receiverData.rPotValue = 0;    
  receiverData.switch1Value = LOW;
  receiverData.switch2Value = LOW;
  receiverData.switch3Value = LOW;
  receiverData.switch4Value = LOW; 
}

void mapAndWriteValues()
{
  servo1.write(map(receiverData.lxAxisValue, 0, 254, 0, 180));
  servo2.write(map(receiverData.lyAxisValue, 0, 254, 0, 180));
  servo3.write(map(receiverData.rxAxisValue, 0, 254, 0, 180));
  servo4.write(map(receiverData.ryAxisValue, 0, 254, 0, 180));
  servo5.write(map(receiverData.lPotValue, 0, 254, 0, 180));
  servo6.write(map(receiverData.rPotValue, 0, 254, 0, 180));
  
  digitalWrite(led1, receiverData.switch1Value);
  digitalWrite(led2, receiverData.switch2Value);
  digitalWrite(led3, receiverData.switch3Value);
  digitalWrite(led4, receiverData.switch4Value); 
}

void setup()
{
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1,pipeIn);
  radio.startListening(); //start the radio receiver 

  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);
  servo5.attach(6);
  servo6.attach(7);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);      
 
  setInputDefaultValues();
  mapAndWriteValues();
}

void loop()
{
  // Check if RF is connected and packet is available 
  if(radio.isChipConnected() && radio.available())
  {
    radio.read(&receiverData, sizeof(PacketData)); 
    lastRecvTime = millis(); 
  }
  else
  {
    //Check Signal lost.
    unsigned long now = millis();
    if ( now - lastRecvTime > SIGNAL_TIMEOUT ) 
    {
      setInputDefaultValues();
    }
  }
  mapAndWriteValues();         
}
