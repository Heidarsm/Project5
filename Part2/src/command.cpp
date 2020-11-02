#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdint.h> // uint8_t
#include<cstdlib>  // atoi()
#include"communicate.h"
#include<iostream>

//slave number, function number, data,
uint16_t modRTU_CRC(uint8_t buf[], int len);
int main(int argc, char *argv[]){

   uint8_t msg[3];
   // *****************************************initialization
   if(argc!=4){
       printf("Invalid number of arguments, exiting!\n");
       return -2;
   }
   communicate test;
   //*********************************************communication

   // populate the message with integer values in binary format
   msg[0] = atoi(argv[1]);
   msg[1] = atoi(argv[2]);
   msg[2] = atoi(argv[3]);

   //calculate CRC for the message
   uint16_t CRC = modRTU_CRC(msg, 3);
   //convert CRC frpm 1 uint16_t to two uint8_t by using register shifting.
   uint8_t CRCpartA = (uint8_t)((CRC & 0xFF00) >> 8);
   uint8_t CRCpartB = (uint8_t)(CRC & 0x00FF);
   printf("CRC1: %d, CRC2: %d \n", CRCpartA, CRCpartB);


   for (int i = 0; i < 5; i++)
   {
      // send the fixed length message with CRC included
      test.send(msg[0],msg[1],msg[2],CRCpartA, CRCpartB);

      usleep(100000);

      test.receve();
      sleep(1);
   }
   

   test.closeConnection();
   return 0;
}

// Compute the MODBUS RTU CRC
uint16_t modRTU_CRC(uint8_t buf[], int len)
{
   //printf("buffer %d %d %d", buf[0], buf[1], buf[2]);
   uint16_t crc = 0xFFFF;
   for (int pos = 0; pos < len; pos++) {
      crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc
   
      for (int i = 8; i != 0; i--) { // Loop over each bit
         if ((crc & 0x0001) != 0) { // If the LSB is set
            crc >>= 1; // Shift right and XOR 0xA001
            crc ^= 0xA001;
         }
         else // Else LSB is not set
            crc >>= 1; // Just shift right
      }
   }  
   // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
   //uint16_t crcSwapped = ((crc & 0xff) << 8) | ((crc & 0xff00) >> 8);
   return crc;
}

