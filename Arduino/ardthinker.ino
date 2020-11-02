#include <Arduino.h>
#include<stdint.h>


const size_t MSG_LEN = 5;
uint8_t msg[MSG_LEN];
uint8_t oldState;

const unsigned char slaveAddress = 0x01;
const unsigned char raspbAddress = 0xA;

int ledPin = 13; // LED with PWM brightness control

//Buffer = [address, function number, data ,crc]

void setup()
{
  // A baud rate of 115200 (8-bit with No parity and 1 stop bit)
  Serial.begin(9600, SERIAL_8N1);
  pinMode(ledPin, OUTPUT); // the LED is an output
}

void loop()
{
  uint8_t address;
  uint8_t functionNumber;
  uint8_t data;

  uint8_t partA;
  uint8_t partB;
  uint8_t partARPi;
  uint8_t partBRPi;
  
  char buffer[100]; // stores the return buffer on each loop

  if (Serial.available() > 0)        // bytes received
  {
    Serial.readBytes(msg, MSG_LEN); // binary messages have fixed length and no terminating \0.

    address = msg[0];
    partARPi = msg[3];
    partBRPi = msg[4];
    
    if (address == slaveAddress)  //Message ment for me !
    {
      functionNumber = msg[1];
      data = msg[2];
      
      analogWrite(ledPin, data); // yes, write out
      
      
      uint16_t crc = modRTU_CRC(msg, MSG_LEN-2);
      partA = (uint8_t)((crc & 0xFF00) >> 8);
      partB = (uint8_t)(crc & 0x00FF);
      
      oldState = stateMachine(oldState);
      data = oldState;
      
      if(partA == partARPi && partB == partBRPi){
        //No errors found checksum adds up
        sprintf(buffer,  "%d %d %d %d %d", raspbAddress, functionNumber, data, partA, partB); 
      }
      else{
        sprintf(buffer,  "Error %d %d %d %d %d %d %d", raspbAddress, functionNumber,data, partA, partB, partARPi, partBRPi);  //String to sent to raspb
      }
        
      

      Serial.print(buffer); // send the buffer to the RPi
    } // otherwise, unknown cmd
    else
    {
      //When this address is not set
      //sprintf(buffer, "Unknown slave: %x", functionNumber);
    }

  }
}
