#include <Arduino.h>

const size_t MSG_LEN = 2;
uint8_t msg[MSG_LEN];

const unsigned char CMD_LED = 0x01;

int ledPin = 13; // LED with PWM brightness control

void setup()
{
   // A baud rate of 115200 (8-bit with No parity and 1 stop bit)
   Serial.begin(115200, SERIAL_8N1);
   pinMode(ledPin, OUTPUT); // the LED is an output
   analogWrite(ledPin, 0);
}

void loop()
{
   uint8_t command;
   char buffer[100]; // stores the return buffer on each loop
   if (Serial.available() > 0)        // bytes received
   {                                  
      Serial.readBytes(msg, MSG_LEN); // binary messages have fixed length and no terminating \0.
      command = msg[0];

      if (command == CMD_LED)
      {
         int level = msg[1];
         if (level >= 0 && level <= 255)
         {                              // is it in range?
            analogWrite(ledPin, level); // yes, write out
            sprintf(buffer, "Set brightness to %d", level);
         }
         else
         { // no, error message back
            sprintf(buffer, "Error: %d is out of range", level);
         }

      } // otherwise, unknown cmd
      else
      {
         sprintf(buffer, "Unknown command: %x", command);
      }
      Serial.print(buffer); // send the buffer to the RPi
   }
}