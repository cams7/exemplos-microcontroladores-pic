/**
--- FUSE Bits ---

Oscillator - XT
Watchdog Timer - Off
Power Up Timer - On
Brown Out Detect - On
Low Voltage Program - Disabled
Data EE Read Protect - Off
Flash Program Write - Write Protection Off
Background Debug - Disabled
Code Protect - Off
*/

#define led PORTB.F1

void main() {
   TRISB = 0xFD;           //0b11111101
   led = 0;
   
   while(1) {
      led = ~led;
      delay_ms(500);
   }
}