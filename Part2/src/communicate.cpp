#include "communicate.h"


communicate::communicate(){
    if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      
   }

   tcgetattr(file, &options);
   cfmakeraw(&options);         // set serial port to raw mode for binary comms

   options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;

   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);
}

void communicate::send(uint8_t slaveID, uint8_t functionNo, uint8_t data, uint8_t CRC1, uint8_t CRC2)
{
    msgToSend[0] = slaveID;
    msgToSend[1] = functionNo;
    msgToSend[2] = data;
    msgToSend[3] = CRC1;
    msgToSend[4] = CRC2;

    if ((count = write(file, msgToSend, 5))<0){
      perror("Failed to write to the output\n");
      
   }
}

void communicate::receve()
{
    
   if ((count = read(file, (void*)receive, 100))<0){
      perror("Failed to read from the input\n");
      
   }

   if (count==0) printf("There was no data available to read!\n");
   else {
      receive[count]=0;  //There is no null character sent by the Arduino
      printf("The following was read in [%d]: %s\n",count,receive);

   }
   
   //*********************************** Problematic part
   // The problem is that we get the  recived portion in a char array pointer.
   // changing that to individual uint8_t values to resend to the nect arduino is proving
   // very hard and I'm not seeing a way that works. We've tried multiple methods but always
   // hit a wall with some cast or conversion.

   // char curr[100];
   // printf("Incoming: ");
   // for(int i = 0; i<100; i++)
   // {
   //    curr[i] = receive[i];
   //    printf("%c",curr[i]);
   // }
   // printf("\n");
   // printf("after %s", curr);
   // char* test;
   // char* test2;

   // test = strtok(curr, " ");
   // test2 = strtok(curr, " ");
   // printf("this is it: %s\n", receive);
   // printf("%c \n", curr[0]);

   //currSlave = curr - '0';
   //currSlave = atoi(curr[0]);
   //printf("%d \n", currSlave);
   
 
}

void communicate::closeConnection()
{
    close(file);
}