#ifndef HEADER_FILE
#define HEADER_FILE

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdint.h> // uint8_t
#include<cstdlib>  // atoi()
#include<iostream>


class communicate {

    public:
        communicate();
        void send(uint8_t slaveID, uint8_t functionNo, uint8_t data, uint8_t CRC1, uint8_t CRC2);
        void receve();
        void closeConnection();

    private:
        
        struct termios options;
        uint8_t msgToSend[5];
        int file;
        int count;
        unsigned char receive[100];
        int currSlave;

};

#endif