/*
*  --- FUSE Bits ---
*  Oscillator           - _XT_OSC
*  Watchdog Timer       - _WDT_OFF
*  Power Up Timer       - _PWRTE_ON
*  Master Clear Enable  - _MCLRE_ON
*  Brown Out Detect     - _BOREN_OFF
*  Low Voltage Program  - _LVP_OFF
*  Data EE Read Protect - _DATA_CP_OFF
*  Code Protect         - _CP_OFF
*/

void nibbleDown(char number);
void nibbleUp(char number);

char counter1 = 0x00, counter2 = 0x00;

void main() {
   CMCON = 0x07;             //0b00000111 - Desabilita comparadores
   TRISB = 0x00;             //0b00000000 - Todo PORTB configurado como saída
   PORTB = 0x00;             //0b00000000 - Inicializa PORTB em nível baixo
   
   while(1){
      counter1++;

      nibbleDown(counter1);
      nibbleUp(counter2);
     
      if(counter1 == 0x0A) {
         counter2++;
         counter1 = 0x00;
         nibbleDown(counter1);
         nibbleUp(counter2);

         if(counter2 == 0x0A) {
            counter2 = 0x00;
            nibbleUp(counter2);
         }
      }

      delay_ms(300);
   }
}

void nibbleDown(char number) {
   //Zera o nibble menos significativo
   //PORTB = 0b???????? & 0b11110000 = 0b????0000
   PORTB &= 0xF0;
   
   //number = 0 -> number = 0b00000000 -> PORTB = 0b????0000 | 0b00000000 = 0b????0000
   //number = 1 -> number = 0b00000001 -> PORTB = 0b????0000 | 0b00000001 = 0b????0001
   //number = 2 -> number = 0b00000010 -> PORTB = 0b????0000 | 0b00000010 = 0b????0010
   //number = 3 -> number = 0b00000011 -> PORTB = 0b????0000 | 0b00000011 = 0b????0011
   //number = 4 -> number = 0b00000100 -> PORTB = 0b????0000 | 0b00000100 = 0b????0100
   //number = 5 -> number = 0b00000101 -> PORTB = 0b????0000 | 0b00000101 = 0b????0101
   //number = 6 -> number = 0b00000110 -> PORTB = 0b????0000 | 0b00000110 = 0b????0110
   //number = 7 -> number = 0b00000111 -> PORTB = 0b????0000 | 0b00000111 = 0b????0111
   //number = 8 -> number = 0b00001000 -> PORTB = 0b????0000 | 0b00001000 = 0b????1000
   //number = 9 -> number = 0b00001001 -> PORTB = 0b????0000 | 0b00001001 = 0b????1001
   PORTB |= number;
}

void nibbleUp(char number) {
   //Zera o nibble mais significativo
   //PORTB = 0b???????? & 0b00001111 = 0b0000????
   PORTB &= 0x0F;
   
   //number = 0 -> number = 0b00000000 << 4 = 0b00000000 -> PORTB = 0b0000???? | 0b00000000 = 0b0000????
   //number = 1 -> number = 0b00000001 << 4 = 0b00010000 -> PORTB = 0b0000???? | 0b00010000 = 0b0001????
   //number = 2 -> number = 0b00000010 << 4 = 0b00100000 -> PORTB = 0b0000???? | 0b00100000 = 0b0010????
   //number = 3 -> number = 0b00000011 << 4 = 0b00110000 -> PORTB = 0b0000???? | 0b00110000 = 0b0011????
   //number = 4 -> number = 0b00000100 << 4 = 0b01000000 -> PORTB = 0b0000???? | 0b01000000 = 0b0100????
   //number = 5 -> number = 0b00000101 << 4 = 0b01010000 -> PORTB = 0b0000???? | 0b01010000 = 0b0101????
   //number = 6 -> number = 0b00000110 << 4 = 0b01100000 -> PORTB = 0b0000???? | 0b01100000 = 0b0110????
   //number = 7 -> number = 0b00000111 << 4 = 0b01110000 -> PORTB = 0b0000???? | 0b01110000 = 0b0111????
   //number = 8 -> number = 0b00001000 << 4 = 0b10000000 -> PORTB = 0b0000???? | 0b10000000 = 0b1000????
   //number = 9 -> number = 0b00001001 << 4 = 0b10010000 -> PORTB = 0b0000???? | 0b10010000 = 0b1001????
   number <<= 4;
   PORTB |= number;
}