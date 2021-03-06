//Compute the MODBUSRTUCRC

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
